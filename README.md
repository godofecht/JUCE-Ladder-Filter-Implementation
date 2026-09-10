# JUCE-Ladder-Filter-Implementation

A ladder filter as a plugin, written as a teaching example.

Two parameters, both automatable:

| Parameter | Range | Default |
|---|---|---|
| `cutoff` | 10 Hz to 20 kHz | 10 Hz |
| `resonance` | 0 to 1 | 0.22 |

Parameters are held in an `AudioProcessorValueTreeState` and read through raw
atomic pointers in `processBlock`, which is the pattern to copy: the message
thread writes through the value tree, the audio thread reads an atomic, and
neither blocks the other. The cutoff is clamped at the low end before it reaches
the filter, because a ladder filter at a cutoff near zero is not stable.

## Build

Open `ShivakumarLadderFilter.jucer` in the Projucer and export for your platform.

See [Analog-Fattener](https://github.com/godofecht/Analog-Fattener) for a
saturation effect built the same way, and
[danzig](https://github.com/godofecht/danzig) for the same problem solved
without JUCE at all.
