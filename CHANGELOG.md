# Changelog

## 0.5.2 — 2026-07-22

- Standardized the build as one Base node and two identical Sentinel nodes.
- Added DHT11 temperature and humidity sensing on XIAO D0 / GPIO1.
- Added DHT11 sampling to the standalone smoke test and Meshtastic overlay.
- Added `GW ENV` command and environmental fields in status and heartbeat messages.
- Added dashboard temperature/humidity parsing, storage, display, demo data, and tests.
- Updated the three-node BOM and authoritative wiring documentation.

## 0.5.1 — 2026-07-22

- Added a double-click Windows GitHub publishing helper.
- Updated the automated commit message for the dashboard gallery and dark-theme additions.
- Re-ran repository verification before packaging.

## 0.5.0 — 2026-07-22

- Added dashboard and mobile concept screenshots in both light and dark themes.
- Added screenshot gallery to the main README.
- Added documentation clarifying the dashboard visual direction and current dark operational theme.
- Updated dashboard docs so the repository presents a clearer product story for the Meshtastic Build-Off.

## 0.4.0 — 2026-07-22

- Added the local GateWatch map dashboard.
- Added GeoJSON import and export, node placement, and zone drawing.
- Added the Meshtastic USB bridge and live dashboard updates.
- Added dashboard server tests and example demo state.

## 0.2.0 — 2026-07-22

- Reworked field node for XIAO ESP32-S3 and bare Wio-SX1262 module.
- Added fence tension-loss input.
- Added PIR motion input with warm-up, debounce, and cooldown protection.
- Added combined `MOTION NEAR BROKEN FENCE` alert.
- Added `GW STATUS` and `GW HELP` Meshtastic commands.
- Added scheduled heartbeat and outbound message spacing.
- Added standalone LoRa hardware smoke test.
- Added desktop simulator and automated tests.
