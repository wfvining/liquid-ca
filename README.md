# Liquid Cellular Automata

[![Build Status](https://travis-ci.com/wfvining/liquid-ca.svg?branch=master)](https://travis-ci.com/wfvining/liquid-ca)

Implements the density classification task in a liquid cellular
automaton.

An example of the simulation running is [here](https://youtu.be/Cbwa5ovKSgU).

## Previous Versions

For the version of this code used in the paper ["How does mobility
help distributed systems
compute?"](https://dx.doi.org/10.1098/rtsb.2018.0375) checkout
`v0.1.1`.

```
$ git checkout v0.1.1
```

## Dependencies

* [SFML](https://www.sfml-dev.org/) is used for the visualization (viz can be disabled 
  if you do not have this installed, spp below).
* pthreads

The code has been built and tested with the viz on Ubuntu 16.05,
18.04, and FreeBSD 11.2. It has been built on OSX without the viz.

## Build
```bash
$ cd liquid-ca
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release .. && make
```

### Build without vizualization

To compile with no vizualization pass `-DBUILD_VIZ=Off` to `cmake`.

### Tests

To run tests do `make test`

## Experiments
In general every experiment is its own executable they take the
following standard options:

| Option                      | Model parameter                      |
| --------------------------- | ------------------------------------ |
| `--num-agents <N>`          | N                                    |
| `--arena-size <L>`          | L                                    |
| `--communication-range <r>` | r                                    |
| `--max-time <T>`            | T                                    |
| `--correlated <sigma>`      | use CRW with sigma std. dev.         |
| `--seed <seed>`             | random seed                          |
| `--by-position`             | initialize agent state by x position |
| `--speed <s>`               | agent speed                          |

Some experiments take additional options.

### Velocity experiment
Basic experiment that evaluates the performance of the LCA for initial
densities in the range [0,1].

`$ ./velocity_experiment <iterations> [options listed above]`

Evaluates each initial density with `iterations` independent random
initial conditions for each initial density.

Outputs the fraction of correctly classified initial conditions for
each initial density.

### Time
`velocity_experiment_time` outputs information about the time to reach
consensus and the mean/median cumulative degree at the moment consensus is
reached. Always runs 100 iterations.

### Visualization
Currently will output a png of the viz every 10 time steps (sorry, I
should make that optional).

`./sim_viz <initial-density> [optons from above]`
