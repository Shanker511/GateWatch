#!/usr/bin/env python3
"""Dependency-free GateWatch state machine used to verify alert behavior."""
from dataclasses import dataclass, field
from typing import List


@dataclass
class GateWatchSim:
    fence_debounce_ms: int = 750
    pir_debounce_ms: int = 200
    pir_warmup_ms: int = 30_000
    pir_cooldown_ms: int = 60_000
    heartbeat_ms: int = 900_000
    boot_ms: int = 0
    fence_raw: bool = False
    fence_stable: bool = False
    fence_changed_ms: int = 0
    pir_raw: bool = False
    pir_stable: bool = False
    pir_changed_ms: int = 0
    last_pir_alert_ms: int = -1
    last_heartbeat_ms: int = 0
    events: List[str] = field(default_factory=list)

    def _debounce(self, name: str, raw: bool, now_ms: int, delay_ms: int) -> bool:
        raw_attr = f"{name}_raw"
        stable_attr = f"{name}_stable"
        changed_attr = f"{name}_changed_ms"
        if raw != getattr(self, raw_attr):
            setattr(self, raw_attr, raw)
            setattr(self, changed_attr, now_ms)
        if raw != getattr(self, stable_attr) and now_ms - getattr(self, changed_attr) >= delay_ms:
            setattr(self, stable_attr, raw)
            return True
        return False

    def sample(self, now_ms: int, fence_broken: bool, pir_active: bool) -> List[str]:
        start = len(self.events)
        if self._debounce("fence", fence_broken, now_ms, self.fence_debounce_ms):
            self.events.append("FENCE BREAK" if self.fence_stable else "FENCE RESTORED")

        if now_ms - self.boot_ms >= self.pir_warmup_ms:
            if self._debounce("pir", pir_active, now_ms, self.pir_debounce_ms) and self.pir_stable:
                cooldown_ok = self.last_pir_alert_ms < 0 or now_ms - self.last_pir_alert_ms >= self.pir_cooldown_ms
                if cooldown_ok:
                    self.last_pir_alert_ms = now_ms
                    self.events.append("MOTION NEAR BROKEN FENCE" if self.fence_stable else "MOTION DETECTED")

        if now_ms - self.last_heartbeat_ms >= self.heartbeat_ms:
            self.last_heartbeat_ms = now_ms
            self.events.append(
                f"HEARTBEAT fence={'BROKEN' if self.fence_stable else 'SECURE'} "
                f"motion={'ACTIVE' if self.pir_stable else 'IDLE'}"
            )
        return self.events[start:]
