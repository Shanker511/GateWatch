# Contributing

GateWatch is in prototype development. Contributions should be reproducible and safety-conscious.

Before opening a pull request:

1. Run `python -m unittest discover -s simulator -v`.
2. Describe the hardware revision and exact pin map used.
3. Include serial logs for radio changes.
4. Do not claim a field result without attaching evidence.
5. Do not add direct energized-fence connections without an isolation and surge-protection review.

Use small commits with clear messages. Keep generated build artifacts out of source control unless they are attached to a tagged release.
