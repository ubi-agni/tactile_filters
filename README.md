# tactile_filters - filters and calibration tools for tactile sensor data

This library stores and computes some internal data statistics to process values smoothed over time, current range statistics and derived values.
It also provides a piece wise linear calibration.

## Single sensor cell filtering

### Available modes

* *rawCurrent*: current raw value, no computation, no averaging
* *rawMean*: averaged raw value
* *absCurrent*: raw value - min value
* *absMean*: averaged absCurrent
* *dynCurrent*: relative value (0-1) within min-max range
* *dynMean*: averaged dynCurrent
* *dynCurrentRelease*: like dynCurrent, but negative values indicate difference force to recently released grasp
* *dynMeanRelease*: averaged dynMeanRelease

## Array sensor filtering

This adds accumulation modes to aggregate all sensor values within the array into a single value.

### Available modes

* Sum
* SumPositive
* SumNegative
* CountPositive
* CountNegative
* Min
* Max

### Settings

* meanAlpha: smoothing factor of the exponential moving average (default 0.7)
* rangeLambda: smoothing factor of the filter for the update of the min and max of the dynamic range (default 0.9995)
* releaseDecay: rate of decay to slowly leave the release mode after entering it (0.05)

## Piece Wise Linear Calibration

The calibration is done according to a calibration file stored in YAML.

### calibration file example for a trapezoid shape

```
0: 0
1: 1
2: 1
3: 0
```
