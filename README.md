# 18-799 Final Project

This was my course project for CMU's 18-799, a speech recognition class. Over
the course of a semester, we developed a recognition engine that could handle
arbitrary number sequences, using both word level and phoneme level models. My
group's recognizer had word error rates of under 10%.

## Acoustic model

Our acoustic model is based on [Mel Frequency Cepstral
Coefficients](https://en.wikipedia.org/wiki/Mel-frequency_cepstrum).
After converting to MFCC form, a set [Gaussian Mixture
Models](https://en.wikipedia.org/wiki/Mixture_model#Gaussian_mixture_model) are
trained for each training instance. We developed models that trained at the word
level, and at the phoneme level.

To train GMMs, each word is naively divided into a few different segments, and
a separate GMM is trained over each segment. Then, the segments are iteratively
reclassified and retrained, to develop a model of the word as it changes over
time.

## Classification

Our language model was a naive directed graph (with support for cycles; this
allows us to detect number sequences of unknown length). This graph is converted
to the transitions of a [Hidden Markov
Model](Mixture_model#Gaussian_mixture_model), and the most likely decoding
estimate is found using [dynamic time
warping](https://en.wikipedia.org/wiki/Dynamic_time_warping).

## Training data

Our training data is a set of recordings of our own voices reading out isolated
numbers, and sequences of numbers.

## Build notes

Must have portaudio installed on your computer to compile. Once portaudio is
configured, the package is self compiling using the make file.

Project is segmented into packages. Makefile autodetects all c files and main
files to determine what executables to compile. Any c files named xxx\_main.c
will be interpreted as a binary and will be compiled to bin/xxx. All
executables assume they will be called from project root, i.e. bin/exec.
