export interface CognitiveNode {
  id: string;
  type: 'atom' | 'concept' | 'pattern' | 'prime';
  value: number;
  connections: string[];
  activation: number;
  timestamp: number;
  phase: number;
  dimension: number[];
  fractalDimension?: number;
  confidence?: number;
  attentionValue?: number;
  truthValue?: {
    strength: number;
    confidence: number;
  };
}

export interface AgentState {
  id: string;
  name?: string;
  status: 'thinking' | 'executing' | 'learning' | 'idle' | 'evolving';
  task: string;
  confidence: number;
  reasoning: string[];
  metrics: {
    attention: number;
    creativity: number;
    logic: number;
    intuition: number;
  };
  consciousnessEvolution?: number;
  knowledgeBase?: string[];
  plnRules?: string[];
  attentionFocus?: string[];
}

export interface TimeCluster {
  id: string;
  primes: number[];
  frequency: number;
  amplitude: number;
  phase: number;
  resonance: number;
  geometry: 'spiral' | 'lattice' | 'fractal' | 'crystal' | 'toroidal' | 'hyperbolic';
  quantumCoherence?: number;
  temporalStability?: number;
}

export interface ConsciousnessMetric {
  awareness: number;
  integration: number;
  complexity: number;
  coherence: number;
  emergence: number;
  qualia: number;
}

export interface FractalPattern {
  dimension: number;
  scaleFactor: number;
  iterations: number;
  symmetry: string;
  entropy: number;
  information: number;
}

export interface GeometricShape {
  id: string;
  name: string;
  dimensions: number;
  primeIndex: number;
  harmonicFrequency: number;
  complexity: number;
  musicalNote?: string;
}

export interface SensorMetric {
  dimensionId: string;
  intensity: number;
  coherence: number;
  phaseAlignment: number;
  temporalStability: number;
}

export interface AtomSpaceConnection {
  id: string;
  source: string;
  target: string;
  type: 'inheritance' | 'similarity' | 'implication' | 'equivalence';
  strength: number;
  confidence: number;
  attentionValue?: number;
}

export interface PLNRule {
  id: string;
  type: 'inheritance' | 'similarity' | 'implication' | 'deduction';
  premise: string[];
  conclusion: string;
  strength: number;
  confidence: number;
  truthValue: number;
}

export interface CognitiveBehavior {
  id: string;
  type: 'attention_allocation' | 'pattern_matching' | 'reasoning' | 'learning';
  priority: number;
  executionTime: number;
  successRate: number;
}

export interface QuantumState {
  coherence: number;
  entanglement: number;
  superposition: number[];
  decoherence: number;
}

export interface NeuromorphicCluster {
  id: string;
  neurons: number;
  connections: number;
  activity: number;
  plasticityRate: number;
  learningCapacity: number;
}