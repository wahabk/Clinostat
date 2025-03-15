
# Version 0.3.1 15/03/2025

- Check for `NULL` input in `calibrate_y_correction()`.

# Version 0.3.0 22/02/2025

- Major refactor. `spin_degs()` and `spin_continuous()` now precompute compensation instead of relying on if statements.
- `calibrate_y_correction()` added to measure `yCorrection` using user input.
- `clockwise` functions removed for direction pin handling.
- `RPM()` removed for now.
- General docs updates.

# Version 0.2.2 09/02/2025

- Add `diagnose_rotation_error()` to measure `yCorrection`.
- Restructure to include `openclino.ino` into the folder `openclino/` so arduino IDE.
- Remove epsilon to `double yCorrection`.
- update `interval` to double and `currentTime` measurements.

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
