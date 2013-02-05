/* initialize_feature_engine - given a window size for our buffers, this
 *                             function allocates space for our new buffers to
 *                             prepare the FFT engine. Buffer lengths must be
 *                             a power of 2. Engine must be initialized before
 *                             running any other functions.
 */
void initialize_feature_engine(int fft_length, int dct_length);

/* premphasize - given a windowed signal, preemphasizes the signal to highboost
 *               it slighty for processing. Change the alpha to change the
 *               amount of high boost.
 */
#define PREMPH_ALPHA .95
void premphasize(double* window);

/* hamming_window - takes an windowed array from our signal and performs a
 *                  hamming weight to smooth the edges to 0
 */
void hamming_window(double* window);

/* hz_to_mel - these pairs of functions convert between actual frequencies
 * mel_to_hz   and mel frequencies
 */
double hz_to_mel(double hz);
double mel_to_hz(double mel);

/* dtft - a wrapper for the library fft code in this project. It handles the
 *       magic to set up the state for the fft, then copies the magnitude of the
 *       result to the output buffer supplied by the user
 */
void dtft(double* input, double* output);

/* dtct - a wrapper for the library dct code in this project. It handles the
 *       magic to set up the state for the dct, then copies the magnitude of the
 *       result to the output buffer supplied by the user
 */
void dtct(double* input, double* output);

/* log_mel_filter - takes in the DTFT of a windowed sample, computes the lcation
 *                  of each mel filter, then runs the DTFT through the mel 
 *                  filters, logs it and stores the output in log_spectra.
 */
void log_mel_filter(double* freqs, int num_filters, double* log_spectra);

/* mel_cepstrum - takes in a log mel spectrum and the number of filters used to
 *                generate it, and runs it through a cosine transform and stores
 *                the desired number of cepstrum points in ceptrum.
 */
void mel_cepstrum(double* log_spectra, int num_filters,
                  double* cepstrum, int num_cepstra);

/* close_feature_engine - clean up after ourselves. Probably not nessecary in
 *                        many cases, only frees memory allocated for buffers.
 */
void close_feature_engine();
