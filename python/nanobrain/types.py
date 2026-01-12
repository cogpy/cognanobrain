"""
Additional helper types for Python

These are pure-Python implementations that complement
the C++ bindings for cases where C++ isn't necessary.
"""

from dataclasses import dataclass, field
from typing import List, Optional, Dict, Any
import math


@dataclass
class TruthValue:
    """Simple Truth Value (strength, confidence, count)."""
    strength: float = 1.0
    confidence: float = 0.0
    count: float = 0.0

    def merge(self, other: 'TruthValue') -> 'TruthValue':
        """Merge two truth values using revision."""
        s1, c1 = self.strength, self.confidence
        s2, c2 = other.strength, other.confidence
        
        count = c1 + c2 - c1 * c2
        if count > 0:
            strength = (s1 * c1 + s2 * c2 - s1 * c1 * c2 * s2) / count
        else:
            strength = 0.5
        
        return TruthValue(strength, count, self.count + other.count)


@dataclass
class AttentionValue:
    """Attention Value (STI, LTI, VLTI)."""
    sti: float = 0.0   # Short-term importance
    lti: float = 0.0   # Long-term importance
    vlti: float = 0.0  # Very-long-term importance


@dataclass
class SimpleAtom:
    """Pure Python atom for prototyping."""
    type: str
    name: str
    truth: TruthValue = field(default_factory=TruthValue)
    attention: AttentionValue = field(default_factory=AttentionValue)
    id: str = ""


# Constants matching C++
FUNDAMENTAL_PRIMES = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]
GOLDEN_RATIO = 1.618033988749895
PI = math.pi
TIME_CRYSTAL_DIMENSIONS = 11


def compute_ppm_coherence(primes: List[int]) -> float:
    """
    Compute Phase Prime Metric coherence.
    
    Matches the C++ implementation formula.
    """
    if not primes:
        return 0.5
    
    product = 1
    total = 0
    
    for p in primes:
        product *= p
        total += p
    
    if total == 0:
        return 0.5
    
    sqrt_product = math.sqrt(product)
    coherence = 0.5 + 0.5 * math.sin(sqrt_product * PI / total)
    
    return coherence


def prime_encode(value: float, num_primes: int = 5) -> List[int]:
    """
    Encode a value as prime signature.
    """
    primes = []
    abs_val = abs(value)
    
    for i in range(num_primes):
        if i < len(FUNDAMENTAL_PRIMES):
            idx = int(abs_val * len(FUNDAMENTAL_PRIMES)) % len(FUNDAMENTAL_PRIMES)
            primes.append(FUNDAMENTAL_PRIMES[idx])
            abs_val = (abs_val * GOLDEN_RATIO) % 1.0
    
    return sorted(set(primes))


def golden_spiral(n: int) -> List[tuple]:
    """
    Generate golden spiral coordinates for visualization.
    """
    points = []
    for i in range(n):
        theta = i * 2 * PI / GOLDEN_RATIO
        r = math.sqrt(i + 1)
        x = r * math.cos(theta)
        y = r * math.sin(theta)
        points.append((x, y))
    return points
