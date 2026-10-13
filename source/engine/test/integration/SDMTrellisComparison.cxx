#include "gtest/gtest.h"

#include "engine/inc/SDMTrellis.h"
#include "engine/inc/PCMUpscale.h"

using namespace omega;

//-------------------------------------------------------------------------------------------
// DSDModSoxTrellisComparator
//-------------------------------------------------------------------------------------------

#define SOX_INT_MIN(bits) (1 <<((bits)-1))
#define SOX_INT_MAX(bits) (((unsigned)-1)>>(33-(bits)))
#define SOX_SAMPLE_MAX (sox_sample_t)SOX_INT_MAX(32)
#define SOX_SAMPLE_MIN (sox_sample_t)SOX_INT_MIN(32)

#define MAX_FILTER_ORDER 8
#define PATH_HASH_SIZE 256
#define PATH_HASH_MASK (PATH_HASH_SIZE - 1)

#define SDM_TRELLIS_MAX_ORDER 32
#define SDM_TRELLIS_MAX_NUM   32
#define SDM_TRELLIS_MAX_LAT   2048

class DSDModSoxTrellisComparator
{
    public:

        typedef int32_t sox_sample_t;

        typedef struct sdm_filter {
            const double  a[MAX_FILTER_ORDER];
            const double  g[MAX_FILTER_ORDER];
            int32_t       order;
            unsigned      freq;
            const char   *name;
            int           trellis_order;
            int           trellis_num;
            int           trellis_lat;
        } sdm_filter_t;

        typedef struct sdm_state {
            double        state[MAX_FILTER_ORDER];
            double        cost;
            uint32_t      path;
            uint8_t       next;
            uint8_t       hist;
            uint8_t       hist_used;
            struct sdm_state *parent;
            struct sdm_state *path_list;
        } sdm_state_t;

        typedef struct {
            sdm_state_t   sdm[2 * SDM_TRELLIS_MAX_NUM];
            sdm_state_t  *act[SDM_TRELLIS_MAX_NUM];
        } sdm_trellis_t;

        typedef struct sdm {
            sdm_trellis_t trellis[2];
            sdm_state_t  *path_hash[PATH_HASH_SIZE];
            uint8_t       hist_free[2 * SDM_TRELLIS_MAX_NUM];
            unsigned      hist_fnum;
            uint32_t      trellis_mask;
            uint32_t      trellis_num;
            uint32_t      trellis_lat;
            unsigned      num_cands;
            unsigned      pos;
            unsigned      pending;
            unsigned      draining;
            unsigned      idx;
            const sdm_filter_t *filter;
            double        prev_y;
            uint64_t      conv_fail;
            uint8_t       hist[2 * SDM_TRELLIS_MAX_NUM][SDM_TRELLIS_MAX_LAT / 8];
        } sdm_t;

        enum sox_error_t {
            SOX_SUCCESS = 0,     /**< Function succeeded = 0 */
            SOX_EOF = -1,        /**< End Of File or other error = -1 */
            SOX_EHDR = 2000,     /**< Invalid Audio Header = 2000 */
            SOX_EFMT,            /**< Unsupported data format = 2001 */
            SOX_ENOMEM,          /**< Can't alloc memory = 2002 */
            SOX_EPERM,           /**< Operation not permitted = 2003 */
            SOX_ENOTSUP,         /**< Operation not supported = 2004 */
            SOX_EINVAL           /**< Invalid argument = 2005 */
        };

    public:
        DSDModSoxTrellisComparator() {}

        bool init(int rate);
        int process(const double *in, int noSamples, uint8_t *out);

        sdm_t *m_mod;

        sdm_filter_t *sdm_find_filter(const char *name, unsigned freq);
        double sdm_filter_calc(const double *s, double *d, const sdm_filter_t *f, double x, double y);
        void sdm_filter_calc2(sdm_state_t *src, sdm_state_t *dst, const sdm_filter_t *f, double x);

        unsigned sdm_histbuf_get(sdm_t *p);
        void sdm_histbuf_put(sdm_t *p, unsigned h);
        unsigned get_bit(uint8_t *p, unsigned i);
        void put_bit(uint8_t *p, unsigned i, unsigned v);
        unsigned sdm_hist_get(sdm_t *p, unsigned h, unsigned i);
        void sdm_hist_put(sdm_t *p, unsigned h, unsigned i, unsigned v);
        void sdm_hist_copy(sdm_t *p, unsigned d, unsigned s);
        int64_t dbl2int64(double a);
        int sdm_cmplt(sdm_state_t *a, sdm_state_t *b);
        int sdm_cmple(sdm_state_t *a, sdm_state_t *b);

        sdm_state_t *sdm_check_path(sdm_t *p, sdm_state_t *s);
        unsigned sdm_sort_cands(sdm_t *p, sdm_trellis_t *st);
        void sdm_step(sdm_t *p, sdm_state_t *cur, sdm_state_t *next, double x);

        sox_sample_t sdm_sample_trellis(sdm_t *p, double x);
        sox_sample_t sdm_sample(sdm_t *p, double x);
        int sdm_process(sdm_t *p, const double *ibuf, sox_sample_t *obuf, size_t *ilen, size_t *olen);
        int sdm_drain(sdm_t *p, sox_sample_t *obuf, size_t *olen);

        sdm_t *sdm_init(const char *filter_name, unsigned freq, unsigned trellis_order, unsigned trellis_num, unsigned trellis_latency);
        void sdm_close(sdm_t *p);

    private:
 
        static constexpr sdm_filter_t sdm_filters[] = {
        {
            {
                1.00323940832478e+00,
                3.54975562370606e-01,
                5.64754047673194e-02,
                3.99067228430322e-03,
            },
            {
                1.74071110561285e-05, 0,
                1.11672812199443e-04, 0,
            },
            4,
            256 * 44100,
            "clans-4",
            0, 0, 0
        },
        {
            {
                8.69746397840960e-01,
                3.58080546314756e-01,
                8.02654082306273e-02,
                8.06528716282692e-03,
            },
            {
                1.74071110561285e-05, 0,
                1.11672812199443e-04, 0,
            },
            4,
            256 * 44100,
            "sdm-4",
            0, 0, 0
        },
        {
            {
                1.10212073518628e+00,
                4.33447134954244e-01,
                7.17865111532609e-02,
                4.48367825425951e-03,
                8.60861641068938e-05,
            },
            {
                0, 4.36651951230006e-05,
                0, 1.23660417994961e-04,
            },
            5,
            256 * 44100,
            "clans-5",
            0, 0, 0
        },
        {
            {
                8.07768375734983e-01,
                3.16440095967511e-01,
                7.38231738259889e-02,
                1.01432044963374e-02,
                6.46658652275506e-04,
            },
            {
                0, 4.36651951230006e-05,
                0, 1.23660417994961e-04,
            },
            5,
            256 * 44100,
            "sdm-5",
            0, 0, 0
        },
        {
            {
                9.97000121097967e-01,
                3.46002867430604e-01,
                5.74352078895161e-02,
                4.96197900435677e-03,
                2.16319301330580e-04,
                3.45938007947910e-06,
            },
            {
                8.57500543083848e-06, 0,
                6.58398680532347e-05, 0,
                1.30939362595793e-04, 0,
            },
            6,
            256 * 44100,
            "clans-6",
            0, 0, 0
        },
        {
            {
                8.08851952379691e-01,
                3.20414766828429e-01,
                7.85858596284593e-02,
                1.24781319607895e-02,
                1.21202847406105e-03,
                5.51622876557856e-05,
            },
            {
                8.57500543083848e-06, 0,
                6.58398680532347e-05, 0,
                1.30939362595793e-04, 0,
            },
            6,
            256 * 44100,
            "sdm-6",
            0, 0, 0
        },
        {
            {
                1.10629931445134e+00,
                4.22135693734657e-01,
                7.54595882135669e-02,
                7.07164815703843e-03,
                3.53092575577382e-04,
                8.89662856104825e-06,
                5.79674109824069e-08,
            },
            {
                0, 2.48046933669715e-05,
                0, 8.28068362972358e-05,
                0, 1.35653594733585e-04,
            },
            7,
            256 * 44100,
            "clans-7",
            0, 0, 0
        },
        {
            {
                7.82785077952658e-01,
                3.01888671316811e-01,
                7.36594376027782e-02,
                1.22068270909817e-02,
                1.36572694403914e-03,
                9.57806082134936e-05,
                3.13239368043838e-06,
            },
            {
                0, 2.48046933669715e-05,
                0, 8.28068362972358e-05,
                0, 1.35653594733585e-04,
            },
            7,
            256 * 44100,
            "sdm-7",
            0, 0, 0
        },
        {
            {
                1.15188624720851e+00,
                5.45054196257555e-01,
                1.38703640845632e-01,
                2.07076444822072e-02,
                1.85506614417771e-03,
                9.63403135615390e-05,
                2.69174565706992e-06,
                2.22594461751768e-08,
            },
            {
                5.06749566262594e-06, 0,
                4.15924517416912e-05, 0,
                9.55783346944871e-05, 0,
                1.38868728742641e-04, 0,
            },
            8,
            256 * 44100,
            "clans-8",
            0, 0, 0
        },
        {
            {
                7.42329617949054e-01,
                2.72509195471757e-01,
                6.41424039739473e-02,
                1.05299412132258e-02,
                1.23178223428228e-03,
                9.94985029720342e-05,
                5.13169547054423e-06,
                1.20466411041020e-07,
            },
            {
                5.06749566262594e-06, 0,
                4.15924517416912e-05, 0,
                9.55783346944871e-05, 0,
                1.38868728742641e-04, 0,
            },
            8,
            256 * 44100,
            "sdm-8",
            0, 0, 0
        },
        {
            {
                1.19985242167687e+00,
                5.39366678861047e-01,
                1.07433710905069e-01,
                7.85649993434925e-03,
            },
            {
                6.96272321944526e-05, 0,
                4.46641365529834e-04, 0,
            },
            4,
            128 * 44100,
            "clans-4",
            0, 0, 0
        },
        {
            {
                8.69935494013007e-01,
                3.57844753369190e-01,
                8.00232187246903e-02,
                7.95176796646842e-03,
            },
            {
                6.96272321944526e-05, 0,
                4.46641365529834e-04, 0,
            },
            4,
            128 * 44100,
            "sdm-4",
            0, 0, 0
        },
        {
            {
                1.12849522129362e+00,
                5.02128177800632e-01,
                1.10084368682902e-01,
                1.18635667860902e-02,
                4.71059243536326e-04,
            },
            {
                0, 1.74653153894942e-04,
                0, 4.94580504383930e-04,
            },
            5,
            128 * 44100,
            "clans-5",
            0, 0, 0
        },
        {
            {
                8.08016362125685e-01,
                3.16129639744972e-01,
                7.34835047943110e-02,
                1.00377576971692e-02,
                6.20309683440734e-04,
            },
            {
                0, 1.74653153894942e-04,
                0, 4.94580504383930e-04,
            },
            5,
            128 * 44100,
            "sdm-5",
            0, 0, 0
        },
        {
            {
                1.13839804508630e+00,
                5.16338264778321e-01,
                1.20760874713903e-01,
                1.53496744585395e-02,
                1.00733946588732e-03,
                2.18223963130981e-05,
            },
            {
                3.42997276004814e-05, 0,
                2.63342132660038e-04, 0,
                5.23688869916463e-04, 0,
            },
            6,
            128 * 44100,
            "clans-6",
            0, 0, 0
        },
        {
            {
                8.09157514480151e-01,
                3.20038611545599e-01,
                7.81955723892726e-02,
                1.23074728674017e-02,
                1.18346416730106e-03,
                5.04301224894810e-05,
            },
            {
                3.42997276004814e-05, 0,
                2.63342132660038e-04, 0,
                5.23688869916463e-04, 0,
            },
            6,
            128 * 44100,
            "sdm-6",
            0, 0, 0
        },
        {
            {
                8.98180853333862e-01,
                3.27985497323439e-01,
                6.38803466871112e-02,
                7.18262647412857e-03,
                4.51845004995476e-04,
                1.49685651672331e-05,
                4.22554681245302e-08,
            },
            {
                0, 9.92163123766340e-05,
                0, 3.31199917300393e-04,
                0, 5.42540771343282e-04,
            },
            7,
            128 * 44100,
            "clans-7",
            0, 0, 0
        },
        {
            {
                7.83148010097334e-01,
                3.01437231238902e-01,
                7.31891646224574e-02,
                1.20314098366875e-02,
                1.32077937193861e-03,
                9.11181979169687e-05,
                2.59895240306562e-06,
            },
            {
                0, 9.92163123766340e-05,
                0, 3.31199917300393e-04,
                0, 5.42540771343282e-04,
            },
            7,
            128 * 44100,
            "sdm-7",
            0, 0, 0
        },
        {
            {
                1.04472698053970e+00,
                4.62088167600438e-01,
                1.13484722685479e-01,
                1.68939738398161e-02,
                1.55891676875336e-03,
                8.23864822188133e-05,
                2.39690238375972e-06,
                -1.75063180618551e-09,
            },
            {
                2.02698799324546e-05, 0,
                1.66362887238597e-04, 0,
                3.82276797905696e-04, 0,
                5.55397776875272e-04, 0,
            },
            8,
            128 * 44100,
            "clans-8",
            0, 0, 0
        },
        {
            {
                7.42763211426562e-01,
                2.71983157679393e-01,
                6.36389361390464e-02,
                1.03289230528372e-02,
                1.19045645863092e-03,
                9.25357160397986e-05,
                4.64982367004083e-06,
                8.14280266547840e-08,
            },
            {
                2.02698799324546e-05, 0,
                1.66362887238597e-04, 0,
                3.82276797905696e-04, 0,
                5.55397776875272e-04, 0,
            },
            8,
            128 * 44100,
            "sdm-8",
            0, 0, 0
        },
        {
            {
                1.27879853057675e+00,
                6.11303913722028e-01,
                1.28497083869344e-01,
                9.36669621421730e-03,
            },
            {
                2.78489536971958e-04, 0,
                1.78576750808173e-03, 0,
            },
            4,
            64 * 44100,
            "clans-4",
            0, 0, 0
        },
        {
            {
                8.70691905361989e-01,
                3.56902669565715e-01,
                7.90540396115068e-02,
                7.49922172520510e-03,
            },
            {
                2.78489536971958e-04, 0,
                1.78576750808173e-03, 0,
            },
            4,
            64 * 44100,
            "sdm-4",
            0, 0, 0
        },
        {
            {
                1.09979653514762e+00,
                4.81149952106030e-01,
                1.03481231987752e-01,
                1.07520561970131e-02,
                3.08801118488355e-04,
            },
            {
                0, 6.98490600683106e-04,
                0, 1.97734357803445e-03,
            },
            5,
            64 * 44100,
            "clans-5",
            0, 0, 0
        },
        {
            {
                8.09008352716413e-01,
                3.14889441429587e-01,
                7.21235639855639e-02,
                9.61769014140330e-03,
                5.16726747047100e-04,
            },
            {
                0, 6.98490600683106e-04,
                0, 1.97734357803445e-03,
            },
            5,
            64 * 44100,
            "sdm-5",
            0, 0, 0
        },
        {
            {
                1.07903996429881e+00,
                4.81889508657128e-01,
                1.12960470418260e-01,
                1.41786764681378e-02,
                8.90696638761455e-04,
                3.12209321540191e-06,
            },
            {
                1.37194204516672e-04, 0,
                1.05309113432481e-03, 0,
                2.09365847953595e-03, 0,
            },
            6,
            64 * 44100,
            "clans-6",
            0, 0, 0
        },
        {
            {
                8.10379824203071e-01,
                3.18536209193388e-01,
                7.66325098232035e-02,
                1.16280270347611e-02,
                1.07113013239551e-03,
                3.23564051386283e-05,
            },
            {
                1.37194204516672e-04, 0,
                1.05309113432481e-03, 0,
                2.09365847953595e-03, 0,
            },
            6,
            64 * 44100,
            "sdm-6",
            0, 0, 0
        },
        {
            {
                1.30828743581024e+00,
                6.14252690035661e-01,
                1.30284958810903e-01,
                1.31280998331490e-02,
                4.80497172614556e-04,
                1.28747977598542e-07,
                -1.01500259908072e-06,
            },
            {
                0, 3.96825873999969e-04,
                0, 1.32436089566069e-03,
                0, 2.16898568341885e-03,
            },
            7,
            64 * 44100,
            "clans-7",
            0, 0, 0
        },
        {
            {
                7.84599817960974e-01,
                2.99634346028983e-01,
                7.13049276218066e-02,
                1.13334107086916e-02,
                1.14497642818158e-03,
                7.33018502803093e-05,
                6.80633400002018e-07,
            },
            {
                0, 3.96825873999969e-04,
                0, 1.32436089566069e-03,
                0, 2.16898568341885e-03,
            },
            7,
            64 * 44100,
            "sdm-7",
            0, 0, 0
        },
        {
            {
                1.18730059129261e+00,
                5.66733317291325e-01,
                1.40117339676942e-01,
                1.87599862200771e-02,
                1.27685506908071e-03,
                8.76397405988154e-06,
                -1.90294986721073e-06,
                -7.39020160622772e-08,
            },
            {
                8.10778762576884e-05, 0,
                6.65340842513387e-04, 0,
                1.52852264942192e-03, 0,
                2.22035724073886e-03, 0,
            },
            8,
            64 * 44100,
            "clans-8",
            0, 0, 0
        },
        {
            {
                7.44453769826547e-01,
                2.69850507860307e-01,
                6.16093616071757e-02,
                9.52771711245796e-03,
                1.02903114196526e-03,
                6.63758229311911e-05,
                2.91124056073927e-06,
                -4.29323230577427e-08,
            },
            {
                8.10778762576884e-05, 0,
                6.65340842513387e-04, 0,
                1.52852264942192e-03, 0,
                2.22035724073886e-03, 0,
            },
            8,
            64 * 44100,
            "sdm-8",
            0, 0, 0
        }, };

    
};

#define sqr(a) ((a) * (a))
#define array_length(a) (sizeof(a)/sizeof(a[0]))
#define min(a, b) ((a) <= (b) ? (a) : (b))

DSDModSoxTrellisComparator::sdm_filter_t *DSDModSoxTrellisComparator::sdm_find_filter(const char *name, unsigned freq)
{
  unsigned i;

  for (i = 0; i < array_length(sdm_filters); i++)
    if (!name || !strcmp(name, sdm_filters[i].name))
      if (sdm_filters[i].freq <= freq)
        return (sdm_filter_t *)&sdm_filters[i];

  return NULL;
}

double DSDModSoxTrellisComparator::sdm_filter_calc(const double *s, double *d, const sdm_filter_t *f, double x, double y)
{
  const double *a = f->a;
  const double *g = f->g;
  double v;
  int i;

  d[0] = s[0] - g[0] * s[1] + x - y;
  v = x + a[0] * d[0];

  for (i = 1; i < f->order - 1; i++) {
    d[i] = s[i] + s[i - 1] - g[i] * s[i + 1];
    v += a[i] * d[i];
  }

  d[i] = s[i] + s[i - 1];
  v += a[i] * d[i];

  return v;
}

void DSDModSoxTrellisComparator::sdm_filter_calc2(sdm_state_t *src, sdm_state_t *dst, const sdm_filter_t *f, double x)
{
  const double *a = f->a;
  double v;
  int i;

  v = sdm_filter_calc(src->state, dst[0].state, f, x, 0.0);

  for (i = 0; i < f->order; i++)
    dst[1].state[i] = dst[0].state[i];

  dst[0].state[0] += 1.0;
  dst[1].state[0] -= 1.0;

  dst[0].cost = src->cost + sqr(v + a[0]);
  dst[1].cost = src->cost + sqr(v - a[0]);
}


inline unsigned DSDModSoxTrellisComparator::sdm_histbuf_get(sdm_t *p)
{
  return p->hist_free[--p->hist_fnum];
}

inline void DSDModSoxTrellisComparator::sdm_histbuf_put(sdm_t *p, unsigned h)
{
  p->hist_free[p->hist_fnum++] = h;
}

inline unsigned DSDModSoxTrellisComparator::get_bit(uint8_t *p, unsigned i)
{
  return (p[i >> 3] >> (i & 7)) & 1;
}

inline void DSDModSoxTrellisComparator::put_bit(uint8_t *p, unsigned i, unsigned v)
{
  int b = p[i >> 3];
  int s = i & 7;
  b &= ~(1 << s);
  b |= v << s;
  p[i >> 3] = b;
}

inline unsigned DSDModSoxTrellisComparator::sdm_hist_get(sdm_t *p, unsigned h, unsigned i)
{
  return get_bit(p->hist[h], i);
}

inline void DSDModSoxTrellisComparator::sdm_hist_put(sdm_t *p, unsigned h, unsigned i, unsigned v)
{
  put_bit(p->hist[h], i, v);
}

inline void DSDModSoxTrellisComparator::sdm_hist_copy(sdm_t *p, unsigned d, unsigned s)
{
  memcpy(p->hist[d], p->hist[s], (size_t)(p->trellis_lat + 7) / 8);
}

inline int64_t DSDModSoxTrellisComparator::dbl2int64(double a)
{
  union { double d; int64_t i; } v;
  v.d = a;
  return v.i;
}

inline int DSDModSoxTrellisComparator::sdm_cmplt(sdm_state_t *a, sdm_state_t *b)
{
  return dbl2int64(a->cost) < dbl2int64(b->cost);
}

inline int DSDModSoxTrellisComparator::sdm_cmple(sdm_state_t *a, sdm_state_t *b)
{
  return dbl2int64(a->cost) <= dbl2int64(b->cost);
}

DSDModSoxTrellisComparator::sdm_state_t *DSDModSoxTrellisComparator::sdm_check_path(sdm_t *p, sdm_state_t *s)
{
  unsigned index = s->path & PATH_HASH_MASK;
  sdm_state_t **hash = p->path_hash;
  sdm_state_t *t = hash[index];

  while (t) {
    if (t->path == s->path)
      return t;
    t = t->path_list;
  }

  s->path_list = hash[index];
  hash[index] = s;

  return NULL;
}

unsigned DSDModSoxTrellisComparator::sdm_sort_cands(sdm_t *p, sdm_trellis_t *st)
{
  sdm_state_t *r, *s, *t;
  sdm_state_t *min;
  unsigned i, j, n;

  for (i = 0; i < 2 * p->num_cands; i++) {
    s = &st->sdm[i];
    p->path_hash[s->path & PATH_HASH_MASK] = NULL;
    if (!i || sdm_cmplt(s, min))
      min = s;
  }

  for (i = 0, n = 0; i < 2 * p->num_cands; i++) {
    s = &st->sdm[i];

    if (s->next != min->next)
      continue;

    if (n == p->trellis_num && sdm_cmple(st->act[n - 1], s))
      continue;

    t = sdm_check_path(p, s);

    if (!t) {
      for (j = n; j > 0; j--) {
        t = st->act[j - 1];
        if (sdm_cmple(t, s))
          break;
        st->act[j] = t;
      }
      if (j < p->trellis_num)
        st->act[j] = s;
      if (n < p->trellis_num)
        n++;
      continue;
    }

    if (sdm_cmple(t, s))
      continue;

    for (j = 0; j < n; j++) {
      r = st->act[j];
      if (sdm_cmple(s, r))
        break;
    }

    st->act[j++] = s;

    while (r != t && j < n) {
      sdm_state_t *u = st->act[j];
      st->act[j] = r;
      r = u;
      j++;
    }
  }

  return n;
}

inline void DSDModSoxTrellisComparator::sdm_step(sdm_t *p, sdm_state_t *cur, sdm_state_t *next, double x)
{
  const sdm_filter_t *f = p->filter;
  int i;

  sdm_filter_calc2(cur, next, f, x);

  for (i = 0; i < 2; i++) {
    next[i].path = (cur->path << 1 | i) & p->trellis_mask;
    next[i].hist = cur->hist;
    next[i].next = cur->next;
    next[i].parent = cur;
  }
}

DSDModSoxTrellisComparator::sox_sample_t DSDModSoxTrellisComparator::sdm_sample_trellis(sdm_t *p, double x)
{
  sdm_trellis_t *st_cur = &p->trellis[p->idx];
  sdm_trellis_t *st_next = &p->trellis[p->idx ^ 1];
  double min_cost;
  unsigned new_cands;
  unsigned next_pos;
  unsigned output;
  unsigned i;

  next_pos = p->pos + 1;
  if (next_pos == p->trellis_lat)
    next_pos = 0;

  for (i = 0; i < p->num_cands; i++) {
    sdm_state_t *cur = st_cur->act[i];
    sdm_state_t *next = &st_next->sdm[2 * i];
    sdm_step(p, cur, next, x);
    cur->next = sdm_hist_get(p, cur->hist, next_pos);
    cur->hist_used = 0;
  }

  new_cands = sdm_sort_cands(p, st_next);
  min_cost = st_next->act[0]->cost;
  output = st_next->act[0]->next;

  for (i = 0; i < new_cands; i++) {
    sdm_state_t *s = st_next->act[i];
    if (s->parent->hist_used) {
      unsigned h = sdm_histbuf_get(p);
      sdm_hist_copy(p, h, s->hist);
      s->hist = h;
    } else {
      s->parent->hist_used = 1;
    }

    s->cost -= min_cost;
    s->next = s->parent->next;
    sdm_hist_put(p, s->hist, p->pos, s->path & 1);
  }

  for (i = 0; i < p->num_cands; i++) {
    sdm_state_t *s = st_cur->act[i];
    if (!s->hist_used)
      sdm_histbuf_put(p, s->hist);
  }

  if (new_cands < p->num_cands)
    p->conv_fail++;

  p->num_cands = new_cands;
  p->pos = next_pos;
  p->idx ^= 1;

  return output ? SOX_SAMPLE_MAX : -SOX_SAMPLE_MAX;
}

DSDModSoxTrellisComparator::sox_sample_t DSDModSoxTrellisComparator::sdm_sample(sdm_t *p, double x)
{
  const sdm_filter_t *f = p->filter;
  double *s0 = p->trellis[0].sdm[p->idx].state;
  double *s1 = p->trellis[0].sdm[p->idx ^ 1].state;
  double y, v;

  v = sdm_filter_calc(s0, s1, f, x, p->prev_y);
  y = signbit(v) ? -1.0 : 1.0;

  p->idx ^= 1;
  p->prev_y = y;

  return y * SOX_SAMPLE_MAX;
}

int DSDModSoxTrellisComparator::sdm_process(sdm_t *p, const double *ibuf, sox_sample_t *obuf, size_t *ilen, size_t *olen)
{
  const double c_gain = 0.5;
  sox_sample_t *out = obuf;
  size_t len = *ilen = min(*ilen, *olen);
  double x;

  if (p->trellis_mask) {
    if (p->pending < p->trellis_lat) {
      size_t pre = min(p->trellis_lat - p->pending, len);
      p->pending += (unsigned int)pre;
      len -= pre;
      while (pre--) {
        x = *ibuf++ * c_gain;
        sdm_sample_trellis(p, x);
      }
    }
    while (len--) {
      x = *ibuf++ * c_gain;
      *out++ = sdm_sample_trellis(p, x);
    }
  } else {
    while (len--) {
      x = *ibuf++ * c_gain;
      *out++ = sdm_sample(p, x);
    }
  }

  *olen = out - obuf;

  return SOX_SUCCESS;
}

int DSDModSoxTrellisComparator::sdm_drain(sdm_t *p, sox_sample_t *obuf, size_t *olen)
{
  if (p->trellis_mask) {
    size_t len = *olen = min(p->pending, *olen);

    if (!p->draining && p->pending < p->trellis_lat) {
      unsigned flush = p->trellis_lat - p->pending;
      while (flush--)
        sdm_sample_trellis(p, 0.0);
    }

    p->draining = 1;
    p->pending -= (unsigned int)len;

    while (len--)
      *obuf++ = sdm_sample_trellis(p, 0.0);
  } else {
    *olen = 0;
  }

  return SOX_SUCCESS;
}

DSDModSoxTrellisComparator::sdm_t *DSDModSoxTrellisComparator::sdm_init(const char *filter_name,
                unsigned freq,
                unsigned trellis_order,
                unsigned trellis_num,
                unsigned trellis_latency)
{
  sdm_t *p;
  const sdm_filter_t *f;
  sdm_trellis_t *st;
  unsigned i;

  if (trellis_order > SDM_TRELLIS_MAX_ORDER) {
    return NULL;
  }

  if (trellis_num > SDM_TRELLIS_MAX_NUM) {
    return NULL;
  }

  if (trellis_latency > SDM_TRELLIS_MAX_LAT) {
    return NULL;
  }

  p = (sdm_t *)malloc(sizeof(*p));
  if (!p)
    return NULL;

  memset(p, 0, sizeof(*p));

  p->filter = sdm_find_filter(filter_name, freq);
  if (!p->filter) {
    return NULL;
  }

  f = p->filter;
  st = &p->trellis[0];

  if (trellis_order || f->trellis_order) {
    if (trellis_order < 1)
      trellis_order = f->trellis_order ? f->trellis_order : 13;

    if (trellis_num)
      p->trellis_num = trellis_num;
    else
      p->trellis_num = f->trellis_num ? f->trellis_num : 8;

    if (trellis_latency)
      p->trellis_lat = trellis_latency;
    else
      p->trellis_lat = f->trellis_lat ? f->trellis_lat : 1024;

    p->trellis_mask = ((uint64_t)1 << trellis_order) - 1;

    for (i = 0; i < 2 * p->trellis_num; i++)
      sdm_histbuf_put(p, i);

    p->num_cands = 1;

    st->sdm[0].hist = sdm_histbuf_get(p);
    st->sdm[0].path = 0;
    st->act[0] = &st->sdm[0];
  }

  return p;
}

void DSDModSoxTrellisComparator::sdm_close(sdm_t *p)
{
  free(p);
}

bool DSDModSoxTrellisComparator::init(int rate)
{
    const char *c_filterName = "sdm-8";
    unsigned int freq = rate * 44100;
    unsigned int order = 8;
    unsigned int nopaths = 32;
    unsigned int latency = 31;

    m_mod = sdm_init(c_filterName, freq, order, nopaths, latency);
    return (m_mod != NULL) ? true : false;
}

int DSDModSoxTrellisComparator::process(const double *in, int noSamples, uint8_t *out)
{
    static const uint8_t msb[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
    size_t noInSamples = noSamples;
    size_t noOutSamples = noSamples;
    sox_sample_t *obuf = new sox_sample_t [noSamples];

    sdm_process(m_mod, in, obuf, &noInSamples, &noOutSamples);

    memset(out, 0, noSamples / 8);
    int dIdx = 0;
    for(int sIdx = 0; sIdx < noOutSamples; sIdx++)
    {
        int bit = sIdx & 0x7;

        if(obuf[sIdx] > 0)
        {
            out[dIdx] |= msb[bit];
        }
        if(bit == 7)
        {
            dIdx++;
        }
    }
    delete [] obuf;
    return (int)noOutSamples / 8;
}

//-------------------------------------------------------------------------------------------

template <typename T> class SDMTrellisCompare : public engine::SDMTrellis<T>
{
	public:
		SDMTrellisCompare();
		virtual ~SDMTrellisCompare();
		virtual bool testIsRateSupported(int rate) const;
		virtual int testCurrentIndexFromNext(int nextPathIdx) const;
		
		engine::SDMTrellisStates<T> *testGetStates(int stateIdx);
		int *stateHashIndex();
		virtual uint32_t testCurrentPath(int pathIdx) const;
		virtual uint32_t testCurrentTrellisState(int pathIdx) const;
		virtual uint32_t testNextPath(int pathIdx) const;
		virtual uint32_t testNextTrellisState(int pathIdx) const;
		virtual int testOutputFromCurrent(int pathIdx) const;
		virtual int testOutputFromNext(int pathIdx) const;
		virtual void testStepPath();
		virtual void testCalc(T sample);
		virtual T testStepMinCostAndResetHash(int& minIdx);
		virtual void testSetNoCandidates(int num);
		virtual int testVirbetiStep();
		virtual int getNoCandidates();
};

//-------------------------------------------------------------------------------------------

template <typename T> SDMTrellisCompare<T>::SDMTrellisCompare()
{}

//-------------------------------------------------------------------------------------------

template <typename T> SDMTrellisCompare<T>::~SDMTrellisCompare()
{}

//-------------------------------------------------------------------------------------------

template <typename T> int SDMTrellisCompare<T>::testCurrentIndexFromNext(int nextPathIdx) const
{
    return this->currentIndexFromNext(nextPathIdx);
}

//-------------------------------------------------------------------------------------------

template <typename T> bool SDMTrellisCompare<T>::testIsRateSupported(int rate) const
{
	return this->isRateSupported(rate);
}

//-------------------------------------------------------------------------------------------

template <typename T> engine::SDMTrellisStates<T> *SDMTrellisCompare<T>::testGetStates(int stateIdx)
{
	return this->m_states[stateIdx];
}

//-------------------------------------------------------------------------------------------

template <typename T> int *SDMTrellisCompare<T>::stateHashIndex()
{
	return this->m_stateHashTable;
}

//-------------------------------------------------------------------------------------------

template <typename T> uint32_t SDMTrellisCompare<T>::testCurrentPath(int pathIdx) const
{
	return this->currentPath(pathIdx);
}

//-------------------------------------------------------------------------------------------

template <typename T> uint32_t SDMTrellisCompare<T>::testCurrentTrellisState(int pathIdx) const
{
	return this->currentTrellisState(pathIdx);
}

//-------------------------------------------------------------------------------------------

template <typename T> uint32_t SDMTrellisCompare<T>::testNextPath(int pathIdx) const
{
	return this->nextPath(pathIdx);
}

//-------------------------------------------------------------------------------------------

template <typename T> uint32_t SDMTrellisCompare<T>::testNextTrellisState(int pathIdx) const
{
	return this->nextTrellisState(pathIdx);
}

//-------------------------------------------------------------------------------------------

template <typename T> int SDMTrellisCompare<T>::testOutputFromCurrent(int pathIdx) const
{
	return this->outputFromCurrent(pathIdx);
}

//-------------------------------------------------------------------------------------------

template <typename T> int SDMTrellisCompare<T>::testOutputFromNext(int pathIdx) const
{
	return this->outputFromNext(pathIdx);
}

//-------------------------------------------------------------------------------------------

template <typename T> void SDMTrellisCompare<T>::testStepPath()
{
	this->stepPath();
}

//-------------------------------------------------------------------------------------------

template <typename T> void SDMTrellisCompare<T>::testCalc(T sample)
{
	this->calc(sample);
}

//-------------------------------------------------------------------------------------------

template <typename T> T SDMTrellisCompare<T>::testStepMinCostAndResetHash(int& minIdx)
{
	return this->stepMinCostAndResetHash(minIdx);
}

//-------------------------------------------------------------------------------------------

template <typename T>void SDMTrellisCompare<T>::testSetNoCandidates(int num)
{
    this->m_noCandidates = num;
}

//-------------------------------------------------------------------------------------------

template <typename T> int SDMTrellisCompare<T>::testVirbetiStep()
{
	return this->vibertiStep();
}

//-------------------------------------------------------------------------------------------

template <typename T> int SDMTrellisCompare<T>::getNoCandidates()
{
	return this->m_noCandidates;
}

//-------------------------------------------------------------------------------------------

void compareSDMTrellisStates(SDMTrellisCompare<double>& mod, DSDModSoxTrellisComparator& modsox)
{
	constexpr double c_tolerance = 0.01;
    DSDModSoxTrellisComparator::sdm_t *pSox = modsox.m_mod;
    DSDModSoxTrellisComparator::sdm_trellis_t *soxTrellis = &(pSox->trellis[pSox->idx]);
	
	EXPECT_EQ(mod.getNoCandidates(), pSox->num_cands);
	for(int stIdx = 0; stIdx < mod.getNoCandidates() && stIdx < (int)pSox->num_cands; stIdx++)
	{
        DSDModSoxTrellisComparator::sdm_state_t *soxState = soxTrellis->act[stIdx];
		ASSERT_TRUE(soxState != NULL);
		engine::SDMTrellisStates<double> *curr = mod.testGetStates(0);
		for(int j = 0; j < 8; j++)
		{
			EXPECT_NEAR(curr->states[j][stIdx], soxState->state[j], c_tolerance);
            if(abs(curr->states[j][stIdx] - soxState->state[j]) > c_tolerance)
            {
                int a = 0;
            }
		}
		EXPECT_NEAR(curr->cost[stIdx], soxState->cost, c_tolerance);
        if(abs(curr->cost[stIdx] - soxState->cost) > c_tolerance)
        {
			int a = 0;
        }
		EXPECT_EQ(curr->path[stIdx] & 0xff, soxState->path);
        if((curr->path[stIdx] & 0xff) != soxState->path)
        {
			int a = 0;;
        }
	}
}

//-------------------------------------------------------------------------------------------

void syncSDMTrellisStates(SDMTrellisCompare<double>& mod, DSDModSoxTrellisComparator& modsox)
{
    DSDModSoxTrellisComparator::sdm_t *pSox = modsox.m_mod;
    DSDModSoxTrellisComparator::sdm_trellis_t *soxTrellis = &(pSox->trellis[pSox->idx]);
    for(int stIdx = 0; stIdx < (int)pSox->num_cands; stIdx++)
    {
        engine::SDMTrellisStates<double> *curr = mod.testGetStates(0);
        DSDModSoxTrellisComparator::sdm_state_t *soxState = soxTrellis->act[stIdx];
        for(int j = 0; j < 8; j++)
        {
            curr->states[j][stIdx] = soxState->state[j];
        }
        curr->cost[stIdx] = soxState->cost;
    }
}

//-------------------------------------------------------------------------------------------

void compareSDMTrellisAlgos(const QString& inFilename, int dsdRate)
{
    int devID = initCUDAOmega();
    ASSERT_TRUE(devID >= 0);

    engine::Codec *inCodec = engine::Codec::get(inFilename);
    ASSERT_FALSE(inCodec == NULL);
    ASSERT_TRUE(inCodec->init());
    ASSERT_EQ(inCodec->noChannels(), 2);

    engine::PCMUpscale upscaleL;
    ASSERT_TRUE(upscaleL.init(inCodec->frequency(), dsdRate));
    engine::PCMUpscale upscaleR;
    ASSERT_TRUE(upscaleR.init(inCodec->frequency(), dsdRate));
    int inputBlockSize = upscaleL.noInputSamples();

    tfloat64 *inL = new tfloat64 [inputBlockSize];
    tfloat64 *inR = new tfloat64 [inputBlockSize];
    int outputLen = upscaleL.noOutputSamples() / 8;
    uint8_t *outL = new uint8_t [outputLen];
    uint8_t *outR = new uint8_t [outputLen];

	DSDModSoxTrellisComparator modsoxL, modsoxR;
	ASSERT_TRUE(modsoxL.init(dsdRate));
	ASSERT_TRUE(modsoxR.init(dsdRate));
	
	SDMTrellisCompare<double> modL, modR;
	ASSERT_TRUE(modL.init(dsdRate, 8, 31));
	ASSERT_TRUE(modR.init(dsdRate, 8, 31));
	engine::SDMTrellisStates<double> *currL = modL.testGetStates(0);
    engine::SDMTrellisStates<double> *nextL = modL.testGetStates(1);
    engine::SDMTrellisStates<double> *currR = modR.testGetStates(0);
    engine::SDMTrellisStates<double> *nextR = modR.testGetStates(1);
	// The SOX paths are set to zero at startup.
	for(int idx = 0; idx < 2 * engine::c_maxNoSDMTrellisPaths; idx++)
	{
		currL->path[idx] = 0;
		nextL->path[idx] = 0;
		currR->path[idx] = 0;
		nextR->path[idx] = 0;
	}
	
    int count = 0;
	engine::RData data(inputBlockSize, inCodec->noChannels(), inCodec->noChannels());	
	bool loop = true;
	while(loop)
	{
		loop = inCodec->next(data);
		if(data.noParts() > 0)
		{
            int idx;

            EXPECT_EQ(data.noParts(), 1);
            sample_t *x = data.partData(0);
            
            for(idx = 0; idx < data.part(0).length(); idx++)
            {
                inL[idx] = x[(idx << 1) + 0];
                inR[idx] = x[(idx << 1) + 1];
            }
            while(idx < inputBlockSize)
            {
                inL[idx] = 0.0;
                inR[idx] = 0.0;
                idx++;
            }

			int outLenL, outLenR;
			QSharedPointer<double> upLPtr = upscaleL.upscale(inL, inputBlockSize, outLenL);
			ASSERT_FALSE(upLPtr.isNull());
			ASSERT_EQ(outLenL / 8, outputLen);
			QSharedPointer<double> upRPtr = upscaleR.upscale(inR, inputBlockSize, outLenR);
			ASSERT_FALSE(upRPtr.isNull());
			ASSERT_EQ(outLenR / 8, outputLen);

            if(count > 10)
            {
                double *upL = upLPtr.get();
                double *upR = upRPtr.get();
			
			    for(idx = 0; idx < outLenL; idx++)
			    {
                    if(count == 13 && idx == 84562)
                    {
                        int a = 0;
                    }

				    int bitLSox = (modsoxL.sdm_sample_trellis(modsoxL.m_mod, upL[idx]) > 0) ? 1 : 0;
				    modL.testCalc(upL[idx]);
				    int bitL = modL.testVirbetiStep();
				    compareSDMTrellisStates(modL, modsoxL);
                    if(bitLSox != bitL)
                    {
                        ASSERT_EQ(bitLSox, bitL);
                    }

				    int bitRSox = (modsoxR.sdm_sample_trellis(modsoxR.m_mod, upR[idx]) > 0) ? 1 : 0;
				    modR.testCalc(upR[idx]);
				    int bitR = modR.testVirbetiStep();
				    compareSDMTrellisStates(modR, modsoxR);
					if(bitRSox != bitR)
					{
						ASSERT_EQ(bitRSox, bitR);
					}

                    if(idx > 0 && (idx % 100) == 0)
                    {
                        syncSDMTrellisStates(modL,modsoxL);
                        syncSDMTrellisStates(modR,modsoxR);
                    }
			    }
            }
			
			data.reset();
		}
		else
		{
			loop = false;
		}
        count++;
	}
	
	delete [] inL;
	delete [] inR;
	delete [] outL;
	delete [] outR;
	delete inCodec;
}

//-------------------------------------------------------------------------------------------

TEST(SDMTrellisComparison, compare64DSD)
{
    QString inFilename = "D:\\Development\\Temp\\dsd\\isla.wav";
    compareSDMTrellisAlgos(inFilename, 64);
}

//-------------------------------------------------------------------------------------------
