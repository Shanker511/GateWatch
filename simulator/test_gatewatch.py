import unittest
from gatewatch_sim import GateWatchSim


class GateWatchTests(unittest.TestCase):
    def test_fence_break_requires_debounce(self):
        sim = GateWatchSim()
        self.assertEqual(sim.sample(100, True, False), [])
        self.assertEqual(sim.sample(800, True, False), [])
        self.assertEqual(sim.sample(851, True, False), ["FENCE BREAK"])

    def test_fence_restoration(self):
        sim = GateWatchSim(fence_raw=True, fence_stable=True)
        sim.sample(1000, False, False)
        self.assertEqual(sim.sample(1800, False, False), ["FENCE RESTORED"])

    def test_pir_warmup_and_cooldown(self):
        sim = GateWatchSim()
        self.assertEqual(sim.sample(1000, False, True), [])
        sim.sample(30000, False, False)
        sim.sample(31000, False, True)
        self.assertEqual(sim.sample(31201, False, True), ["MOTION DETECTED"])
        sim.sample(32000, False, False)
        sim.sample(32201, False, False)
        sim.sample(33000, False, True)
        self.assertEqual(sim.sample(33201, False, True), [])

    def test_combined_risk_alert(self):
        sim = GateWatchSim(fence_raw=True, fence_stable=True)
        sim.sample(31000, True, True)
        self.assertEqual(sim.sample(31201, True, True), ["MOTION NEAR BROKEN FENCE"])

    def test_heartbeat(self):
        sim = GateWatchSim()
        self.assertEqual(sim.sample(900000, False, False), ["HEARTBEAT fence=SECURE motion=IDLE"])


if __name__ == '__main__':
    unittest.main()
