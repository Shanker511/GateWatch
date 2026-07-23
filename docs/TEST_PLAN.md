# Test Plan

Record every test with date, firmware commit, node name, power source, antenna, distance, weather, result, and photo/video reference.

## Bench tests

1. Boot with fence switch closed. Verify `SECURE` and no false break event.
2. Open switch for less than debounce time. Verify no alert.
3. Hold switch open. Verify one `FENCE BREAK` alert.
4. Close switch. Verify one `FENCE RESTORED` alert.
5. Trigger PIR after warm-up. Verify one `MOTION DETECTED` alert.
6. Retrigger PIR during cooldown. Verify no message flood.
7. Open fence switch, then trigger PIR. Verify `MOTION NEAR BROKEN FENCE`.
8. Send `GW STATUS`. Verify direct status reply.
9. Leave operating for at least one heartbeat interval. Verify status heartbeat.
10. Power-cycle with fence open. Verify startup state is reported correctly after initialization.

## Field tests

- Test at 25 m, 100 m, 250 m, and a realistic property distance.
- Test normal line of sight and with terrain/building obstruction.
- Operate for at least four hours on the intended battery.
- Pull, release, and repair the mechanical test fence several times.
- Note false triggers from wind, livestock contact, thermal changes, and enclosure movement.

## Pass criteria

- Every sustained break produces exactly one break alert within the configured timing window.
- Every restoration produces exactly one restoration alert.
- PIR events remain rate-limited.
- Status command receives a reply.
- No resets or radio initialization failures occur during the field session.
