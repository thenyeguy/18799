#include <float.h>
#include <stdbool.h>


/* NOTE: current code is configured to maximize score. Changing it to minimize
 *       cost instead should be simple, but requires changes in a fwe places */
#define DTW_MAX_SCORE DBL_MAX
#define DTW_MIN_SCORE DBL_MIN


/* Pruning type for the trellis evalutation...
 *     NONE performs no pruning
 *     BEAM rules out any evaluation that exceeds the best score by threshold
 * Threshold for pruning is a multiplicative factor. For example, if we use
 *     beam pruning with a threshold of 3, and our max column score is 15, then
 *     we prune out any node with a score less than (15/3 == 5)
 */
typedef enum {
    DTW_NO_PRUNE,
    DTW_BEAM_PRUNE
} dtw_prune_t;
#define DTW_THRESHOLD 3


/* Direction type - for indicating path directions */
typedef enum {
    DTW_DIR_NONE,
    DTW_DIR_LEFT,
    DTW_DIR_DOWNONE,
    DTW_DIR_DOWNTWO
} dtw_trellis_dir;


/* Node type - for capturing a single node in the dtw */
typedef struct {
	double score;
    bool pruned;
    dtw_trellis_dir dir;
} dtw_trellis_node;


/* DTW helper struct - encapsulates the DTW information
 * Keeps track of the template and evaluation data. Columns represent time and
 *     rows in each column represent position in the template.
 * Holds a function pointer to the score function, that given a direction
 *     and the template/data node data, returns a score for that transition.
 *     PROTOTYPE:
 *         double scorer(void* test_node, void* template_node, dtw_dir dir);
 * The last_col contains the most recently scored column of data, up to the
 *     last_column_ith element of the test_data.
 * The next_col holds stats on whether the next node is pruned, and always has
 *     a score of MIN_SCORE. It is filled by fill_next_col when called.
 */
typedef struct {
    void* template_data;
    int template_length;
    void* test_data;
    int test_length;

    dtw_prune_t pruning;
    double (*scorer)(void*,void*,dtw_trellis_dir);

    int last_column_i;
    dtw_trellis_node* last_col;
    dtw_trellis_node* next_col;
    double score;
} dtw_t;


/* new_dtw - given the template and test data to compare, as well as the pruning
 *           method and function to score a transition, creates a new dtw struct
 *           to perform the dtw.
 *
 *           NOTE: If there is to be a dummy node at the beginning of the test
 *           or template data, then the user must preface the template/test
 *           array with it themself, and the scorer is responsible for scoring
 *           it accordingly.
 *
 *           NOTE: The scorer must also hold information for DIR_NONE, ie at the
 *           very beginning, what is the penalty for starting at any node of the
 *           template
 */
dtw_t* new_dtw(void* template_data, int template_length,
               void* test_data,     int test_length,
               dtw_prune_t prune, double (*scorer)(void*,void*,dtw_trellis_dir));


/* dtw_fill_next_column - given a dtw struct, first checks to see if 
 *                    last_column_i == test_length... If it is, we have filled
 *                    out the entire trellis, and it returns true. Otherwise,
 *                    fills the next column of the search trellis and returns
 *                    false to say we are not done. Prunes using the specified
 *                    method in the dtw_t struct.
 */
bool dtw_fill_next_col(dtw_t* dtw);


/* dtw_score_node - Helper for dtw_fill_next_col... Given a dtw and the row
 *                  number, scores that row of next_col using information
 *                  from last_col. Stores result in the dtw struct and returns
 *                  the score.
 */
double dtw_score_node(dtw_t* dtw, int n);


/* print_dtw_trellis - given a dtw structure, prints out stats in human readable
 *                     format for debugging/insight.
 */
void dtw_print_trellis(dtw_t* dtw);


/* print_dtw_col - given a dtw structure, prints out the last scored column in
 *                 human readable format. Counterinuitively prints it out in
 *                 a row, because we don't know how many columns there are as
 *                 we overwrite at each step.
 */
void dtw_print_col(dtw_t* dtw);


/* free_dtw - frees the given dtw structure, but NOT the template and test data,
 *            as that is the user's responsibility
 */
void free_dtw(dtw_t* dtw);
