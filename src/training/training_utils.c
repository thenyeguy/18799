#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "training_utils.h"
#include "../grammar/grammar.h"
#include "../grammar/viterbi.h"


gaussian_cluster** train_from_recordings(feature_vectors** recordings,
        int num_recordings, gaussian_cluster*** models, int* num_models,
        char** words_in_corpus, int num_words)
{
    /* Count number of each word in our corpus
     */
    int* templates_per_word = calloc(num_words,sizeof(int));

    for(int i = 0; i < num_recordings; i++)
    {
        for(int j = 0; j < num_models[i]; j++)
        {
            //Figure out what index this word takes on
            char* word = models[i][j]->word_id;
            for(int k = 0; k < num_words; k++)
            {
                if(strcmp(word, words_in_corpus[k]) == 0)
                    templates_per_word[k]++;
            }
        }
    }


    /* Create our array of templates for clustering
     * Use templates_added to keep track of our counts in each set so far
     */
    int* templates_added = calloc(num_words,sizeof(int));
    feature_vectors*** all_templates =
        calloc(num_words,sizeof(feature_vectors**));
    for(int i = 0; i < num_words; i++)
    {
        all_templates[i] =
            calloc(templates_per_word[i], sizeof(feature_vectors*));
    }

    /* For each recording, get segment our features from the models
     * And add them to our template collection
     */
    for(int i = 0; i < num_recordings; i++)
    {
        feature_vectors** temps =
            extract_words_from_recording(recordings[i], models[i],
                                         num_models[i]);

        //For each vector segmented out, add it to one template collection
        for(int j = 0; j < num_models[i]; j++)
        {
            //Determine which word this template is
            for(int k = 0; k < num_words; k++)
            {
                if(strcmp(temps[j]->word_id, words_in_corpus[k]) == 0)
                {
                    all_templates[k][templates_added[k]] = temps[j];
                    templates_added[k]++;
                    break;
                }
            }
        }
    }


    /* Cluster our results
     */
    gaussian_cluster** final_clusters =
        calloc(num_words,sizeof(gaussian_cluster));
    for(int i = 0; i < num_words; i++)
    {
        final_clusters[i] =
            cluster_templates(all_templates[i], templates_per_word[i],
                              NUM_CLUSTERS, words_in_corpus[i]);
    }


    /* Clean up
     */
    free(templates_per_word);
    free(templates_added);
    for(int i = 0; i < num_words; i++)
    {
        free(all_templates[i]);
    }
    free(all_templates);


    return final_clusters;
}


feature_vectors** extract_words_from_recording(feature_vectors* recording,
        gaussian_cluster** models, int num_models)
{
    /* Construct a linear grammar from our models - one leads directly to another
     */
    grammar* grammar = malloc(sizeof(struct grammar));
    grammar->nodes = calloc(num_models+1, sizeof(struct grammar_node));
    grammar->hmms = models;
    grammar->num_nodes = num_models + 1;
    grammar->num_edges = num_models;
    grammar->num_hmms = num_models;

    //Fill each node and edge with one model
    for(int i = 0; i < num_models; i++)
    {
        grammar_node* node = &(grammar->nodes[i]);
        grammar_transition* edge = malloc(sizeof(struct grammar_transition));

        node->node_id = i;
        node->num_edges = 1;
        node->edges = edge;
        node->terminal = false;

        edge->transition_prob = 1.0;
        edge->hmm = models[i];
        edge->hmm_id = i;
        edge->next_node_id = i+1;
    }

    //Fill the receiving node
    grammar_node* node = &grammar->nodes[num_models];
    node->node_id = num_models;
    node->num_edges = 0;
    node->edges = NULL;
    node->terminal = true;


    /* Get the backtrace to find the segmentation times of our models.
     */
    int* segmentation_times = calloc(num_models+1, sizeof(int));
    backpointer* tail = viterbi_backtrace(grammar, recording, 10000.0);

    if(strcmp(tail->word, "NO RESULTS") == 0)
    {
        printf("Endpointing failed. Aborting.\n");
        exit(1);
    }

    //Fill array from backtrace
    backpointer* p = tail;
    for(int i = num_models; i >= 0; i--)
    {
        segmentation_times[i] = p->timestamp;
        p = p->prev;
    }


    /* From our segment times, break out our recording into its parts
     */
    feature_vectors** result = calloc(num_models, sizeof(feature_vectors*));
    for(int i = 0; i < num_models; i++)
    {
        feature_vectors* part = malloc(sizeof(feature_vectors));
        part->num_vectors = segmentation_times[i+1] - segmentation_times[i] + 1;
        part->features = &recording->features[segmentation_times[i]];
        strcpy(part->word_id, models[i]->word_id);
        result[i] = part;
    }


    /*Clean up
     */
    for(int i = 0; i < num_models; i++)
    {
        free(grammar->nodes[i].edges);
    }
    free(grammar);
    free(segmentation_times);

    return result;
}

