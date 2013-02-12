// Define constants for the windowing in ms

// WINDOW_WIDTH is how wide the window is, and WINDOW_SLIDE is how fary each
// adjacent window will shift over
#define WINDOW_WIDTH 20
#define WINDOW_SLIDE 10


// Define struct to store cepstrum vector information in

struct cepstrum_vectors
{
    int num_vectors; // Total number of vectors from windows
    int num_points;  // Number of sample points per ceptsrum vector
    double** cepstra; // An array of cepstrum vectors
};
typedef struct cepstrum_vectors cepstrum_vectors;


// Define function interface

/* log_* - the log functions simply take in a computed array for a single window
 *         of the specified value and output it to a corresponding file
 */
void log_window(char* file_prefix, double* window, int n);
void log_dtft(char* file_prefix, double* dtft, int n);
void log_logmel(char* file_prefix, double* logmel, int n);
void log_cepstrum(char* file_prefix, double* cepstrum, int n);

/* cepstrum_vectors - takes in a signal and its length, and returns a struct
 *                    of the ceptrum vectors computed with the number of filters
 *                    and sample points as provided. Output result contains
 *                    a matrix of cepstrum values for signal.
 *                      
 *                    If file_prefix is non-null, then outputs
 */
cepstrum_vectors* get_cepstrum_vectors(double* signal, int num_samples,
                                       int num_mel_filters, int num_cepstra,
                                       char* file_prefix);
