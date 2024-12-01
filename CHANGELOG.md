
# Version 0.2.1 01/12/2024

- Fix `spin_continuous()` to not use `AccelStepper` and to compensate motors for each other.
- Reset `epsilon` to 1.

# Version 0.2.0 29/11/2024

- Changed pulley diameters to pulley teeth
- Added `float epsilon` for calibrating substeps.
- Added circuit diagram.
- Updated documentation and README.md
- updated `spin_degs()` if statements and compensation.
- Added unit tests `calibrate_pulley_teeth()` and `test_spin_degs_multi()`.

# Version 0.1.0 22/11/2024

Initial release. Development till 22/11/2024.
