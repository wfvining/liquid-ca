#!/usr/bin/env bash

NUM_AGENTS=127
SEED=2345

# super-difusive
for communication_radius in 1 2 5 7 14 25 32 63
do
    ../levy_walk 0.2                        \
                 --num-agents ${NUM_AGENTS} \
                 --arena-size ${NUM_AGENTS} \
                 --seed       ${SEED}
                 --communication-range ${communication_radius} > ../cr-${communication_radius}_0.2_results.txt
done

# mid range
for communication_radius in 1 2 5 7 14 25 32 63
do
    ../levy_walk 1.2                        \
                 --num-agents ${NUM_AGENTS} \
                 --arena-size ${NUM_AGENTS} \
                 --seed       ${SEED}       \
                 --communication-range ${communication_radius} > ../cr-${communication_radius}_1.2_results.txt
done

# brownian
for communication_radius in 1 2 5 7 14 25 32 63
do
    ../levy_walk 1.99                       \
                 --num-agents ${NUM_AGENTS} \
                 --arena-size ${NUM_AGENTS} \
                 --seed ${SEED}             \
                 --communication-range ${communication_radius} > ../cr-${communication_radius}_1.2_results.txt
done
