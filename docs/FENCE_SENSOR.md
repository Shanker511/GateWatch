# Mechanical Fence Sensor

## Principle

Barbed wire is a poor supervised electrical conductor because it contacts ground, vegetation, staples, posts, moisture, corrosion, and other fence strands. GateWatch instead detects the mechanical result of a break: loss of tension.

## Mechanism

```text
barbed wire -> approved fence clamp -> extension spring -> lever -> NC roller microswitch
```

Normal tension holds the microswitch closed. A break, released clamp, or severe slack allows the spring and lever to move, opening the switch.

## Adjustment

- Mount the mechanism at a brace or termination post.
- Use an adjustment bolt or slotted bracket to set the trigger point.
- Keep the microswitch and electrical terminations inside a weather-resistant box.
- Do not make the small switch carry the fence's full structural load. A separate mechanical stop must absorb excessive tension.
- Divide long fences into named sections; one switch reports the affected section, not the exact break location.

Suggested node names include `GW-NORTH-01`, `GW-WEST-02`, and `GW-EAST-GATE`.
