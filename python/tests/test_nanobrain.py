"""
NanoBrain Python Tests

Run with: pytest tests/
"""

import pytest


# Note: These tests require the C++ bindings to be built
# Skip if bindings not available

try:
    import nanobrain as nb
    HAS_BINDINGS = True
except ImportError:
    HAS_BINDINGS = False


@pytest.mark.skipif(not HAS_BINDINGS, reason="C++ bindings not built")
class TestVersion:
    """Test version information."""
    
    def test_version_string(self):
        assert nb.__version__ == "1.0.0"
    
    def test_get_version(self):
        assert nb.get_version() == "1.0.0"


@pytest.mark.skipif(not HAS_BINDINGS, reason="C++ bindings not built")
class TestConstants:
    """Test exported constants."""
    
    def test_fundamental_primes(self):
        primes = nb.FUNDAMENTAL_PRIMES
        assert len(primes) == 15
        assert primes[0] == 2
        assert primes[1] == 3
        assert primes[14] == 47
    
    def test_dimensions(self):
        assert nb.TIME_CRYSTAL_DIMENSIONS == 11
    
    def test_golden_ratio(self):
        assert abs(nb.GOLDEN_RATIO - 1.618033988749895) < 0.0001


@pytest.mark.skipif(not HAS_BINDINGS, reason="C++ bindings not built")
class TestUnifiedKernel:
    """Test UnifiedKernel class."""
    
    def test_create_config(self):
        config = nb.UnifiedConfig()
        assert config.time_crystal_dimensions == 11
    
    def test_modify_config(self):
        config = nb.UnifiedConfig()
        config.debug_output = True
        assert config.debug_output == True
    
    def test_kernel_lifecycle(self):
        config = nb.UnifiedConfig()
        kernel = nb.UnifiedKernel(config)
        
        assert not kernel.is_active()
        kernel.initialize()
        assert kernel.is_active()
        kernel.shutdown()
        assert not kernel.is_active()
    
    def test_create_atom(self):
        config = nb.UnifiedConfig()
        kernel = nb.UnifiedKernel(config)
        kernel.initialize()
        
        atom_id = kernel.create_atom("ConceptNode", "Cat", 0.9, 0.8)
        assert atom_id is not None
        assert len(atom_id) > 0
        
        atom = kernel.get_atom(atom_id)
        assert atom is not None
        assert atom.name == "Cat"
        
        kernel.shutdown()
    
    def test_get_metrics(self):
        config = nb.UnifiedConfig()
        kernel = nb.UnifiedKernel(config)
        kernel.initialize()
        
        kernel.create_atom("ConceptNode", "Dog", 0.85, 0.9)
        
        metrics = kernel.get_metrics()
        assert metrics.total_atoms >= 1
        
        kernel.shutdown()


@pytest.mark.skipif(not HAS_BINDINGS, reason="C++ bindings not built")
class TestTimeCrystal:
    """Test TimeCrystalKernel class."""
    
    def test_create_config(self):
        config = nb.TimeCrystalConfig()
        assert config.quantum_coherence_threshold > 0
    
    def test_kernel_lifecycle(self):
        config = nb.TimeCrystalConfig()
        tc = nb.TimeCrystalKernel(config)
        
        tc.initialize()
        assert tc.is_active()
        tc.shutdown()
        assert not tc.is_active()
    
    def test_ppm_coherence(self):
        config = nb.TimeCrystalConfig()
        tc = nb.TimeCrystalKernel(config)
        tc.initialize()
        
        coherence = tc.compute_ppm_coherence([2, 3, 5, 7])
        assert 0.0 <= coherence <= 1.0
        
        tc.shutdown()


@pytest.mark.skipif(not HAS_BINDINGS, reason="C++ bindings not built")
class TestTCTransform:
    """Test TCTransformEngine class."""
    
    def test_transform(self):
        config = nb.UnifiedConfig()
        kernel = nb.UnifiedKernel(config)
        kernel.initialize()
        
        tc_config = nb.TCTransformConfig()
        transform = nb.TCTransformEngine(kernel, tc_config)
        
        signal = [0.1, 0.2, 0.3, 0.4, 0.5, 0.4, 0.3, 0.2, 0.1]
        result = transform.transform(signal)
        
        assert len(result.prime_spectrum) > 0
        assert len(result.dominant_primes) > 0
        assert 0.0 <= result.overall_coherence <= 1.0
        
        kernel.shutdown()


@pytest.mark.skipif(not HAS_BINDINGS, reason="C++ bindings not built")
class TestGOG:
    """Test GardenOfGardens class."""
    
    def test_lifecycle(self):
        config = nb.UnifiedConfig()
        kernel = nb.UnifiedKernel(config)
        kernel.initialize()
        
        gog_config = nb.GOGConfig()
        gog = nb.GardenOfGardens(kernel, gog_config)
        
        gog.initialize()
        assert gog.is_active()
        
        garden_id = gog.create_garden("test_garden")
        assert len(garden_id) > 0
        
        metrics = gog.get_metrics()
        assert metrics.total_gardens >= 1
        
        gog.shutdown()
        kernel.shutdown()


# Basic import test (always runs)
def test_import():
    """Test that package can be imported."""
    try:
        import nanobrain
        assert hasattr(nanobrain, '__version__')
    except ImportError:
        pytest.skip("Bindings not built")
