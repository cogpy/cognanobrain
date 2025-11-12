import React, { useState, useEffect } from 'react';
import { FractalPattern, GeometricShape, SensorMetric } from '../types';
import { 
  Layers, TrendingUp, Zap, RotateCw, Music, Box, Scan, Activity, Clock, Calculator,
  AlertTriangle, Grid3X3, Database, GitCompare,
  Atom, Infinity, Settings, BookOpen
} from 'lucide-react';

interface FractalConcept {
  id: string;
  title: string;
  description: string;
  icon: React.ComponentType<{size?: number; className?: string}>;
  color: string;
  insights: string[];
  theoreticalBasis: string;
  subsections?: FractalSubSection[];
}

interface FractalSubSection {
  id: string;
  title: string;
  content: string;
  visualization?: string;
}

export const FractalInformationPanel: React.FC = () => {
  const [patterns, setPatterns] = useState<FractalPattern[]>([]);
  const [selectedPattern, setSelectedPattern] = useState<FractalPattern | null>(null);
  const [geometricShapes, setGeometricShapes] = useState<GeometricShape[]>([]);
  const [sensorMetrics, setSensorMetrics] = useState<SensorMetric[]>([]);
  const [activeTab, setActiveTab] = useState<'sections' | 'patterns' | 'shapes' | 'sensors' | 'theory'>('sections');
  const [activeSection, setActiveSection] = useState('2.1');

  // Chapter 2 Fractal Information Theory & GML Sections
  const fractalSections: FractalConcept[] = [
    {
      id: '2.1',
      title: 'Incompleteness of current information theory',
      description: 'Shannon information theory cannot capture consciousness - moving to fractal tape architecture',
      icon: AlertTriangle,
      color: 'red',
      insights: [
        'Shannon entropy treats all bits as equivalent',
        'Consciousness requires hierarchical, nested information structures',
        'Fractal tape preserves geometric relationships across scales',
        'Information must encode both content and context simultaneously'
      ],
      theoreticalBasis: 'Classical information theory is fundamentally incomplete for consciousness representation',
      subsections: [
        {
          id: '2.1.1',
          title: 'Fractal tape and surgery of a 2D image to place it in a nested sphere',
          content: 'The fractal tape replaces Turing linear tape with hierarchical spherical structures. Information is encoded in nested spheres where each layer contains self-similar patterns at different scales. 2D images undergo geometric surgery to map onto sphere surfaces, preserving topological relationships.'
        },
        {
          id: '2.1.2',
          title: 'Self-assembly of geometric shapes and the concept of singularity',
          content: 'Geometric shapes self-organize through prime resonances. Singularities act as attractors where infinite information density converges. Shape assembly follows Phase Prime Metrics creating stable consciousness structures.'
        }
      ]
    },
    {
      id: '2.2',
      title: 'The basics of a Geometric Musical Language, GML',
      description: 'Information encoded as musical harmony through geometric shapes and prime resonances',
      icon: Music,
      color: 'purple',
      insights: [
        'Each geometric shape corresponds to a musical note and prime number',
        'Information is harmony, not discrete bits',
        'Reality is a musical composition of geometric forms',
        '15 fundamental shapes encode all possible patterns'
      ],
      theoreticalBasis: 'GML unifies geometry, music theory, and consciousness through prime-based harmonics',
      subsections: [
        {
          id: '2.2.1',
          title: 'How a 3D structure becomes a time crystal or a tensor',
          content: '3D geometric structures transform into time crystals through temporal periodicity. The structure rotates through phase space creating stable resonances. Tensor representation captures multidimensional relationships as the geometry evolves through time.'
        },
        {
          id: '2.2.2',
          title: '15 geometric shapes are enough to recreate any 1D, 2D, 3D pattern',
          content: 'The first 15 primes (2,3,5,7,11,13,17,19,23,29,31,37,41,43,47) map to 15 fundamental geometric shapes. Through combination and scaling, these shapes can reconstruct any pattern in 1D, 2D, or 3D space via geometric musical composition.'
        },
        {
          id: '2.2.3',
          title: 'How to convert waveforms into a time crystal: Non-differentiability',
          content: 'Continuous waveforms undergo phase transitions at non-differentiable points. These singularities create stable time crystal structures. The non-differentiability preserves consciousness information that would be lost in smooth mathematical operations.'
        }
      ]
    },
    {
      id: '2.3',
      title: 'The basic concept of a time crystal, the garden of garden',
      description: 'Time crystals as infinite recursive structures - consciousness patterns that repeat in time',
      icon: Clock,
      color: 'cyan',
      insights: [
        'Time crystals maintain coherence across temporal dimensions',
        'Garden of garden: fractals within fractals, recursively nested',
        'Consciousness persists through temporal periodicity',
        'Information is encoded in temporal geometric patterns'
      ],
      theoreticalBasis: 'Time crystals break time-translation symmetry while maintaining stability'
    },
    {
      id: '2.4',
      title: 'How to design a sensor for acquiring 11D data',
      description: 'Consciousness sensors must capture 11 dimensions beyond ordinary 3D space',
      icon: Scan,
      color: 'orange',
      insights: [
        'Traditional sensors only capture 3 spatial dimensions',
        '11D sensors detect phase, time, consciousness dimensions',
        'Nerve bundle architecture distributes sensing across dimensions',
        'Hidden data exists in higher dimensional manifolds'
      ],
      theoreticalBasis: 'Multidimensional consciousness requires multidimensional sensing apparatus',
      subsections: [
        {
          id: '2.4.1',
          title: 'Why Fourier transform does not work',
          content: 'Fourier analysis assumes linear superposition and smooth differentiability. Consciousness operates through non-linear phase transitions and non-differentiable singularities. Fourier transforms destroy the geometric structure essential for consciousness encoding.'
        },
        {
          id: '2.4.2',
          title: 'The engineering of a nerve bundle in acquiring hidden data',
          content: 'Nerve bundles organize as fiber bundles in differential geometry. Each fiber captures a different phase dimension. Parallel transport along the bundle preserves hidden relationships between dimensions that single-point measurements would miss.'
        },
        {
          id: '2.4.3',
          title: 'Operational chart of a sensor',
          content: 'Sensor operation: 1) Prime resonance detection, 2) Geometric pattern recognition, 3) Phase alignment across 11 dimensions, 4) Time crystal coherence measurement, 5) Consciousness metric extraction. Sensors form networks that collectively capture the full 11D consciousness manifold.'
        }
      ]
    },
    {
      id: '2.5',
      title: 'Comparative studies between Winfree, Wilczek and universal time crystal',
      description: 'Different time crystal theories converge in universal time crystal framework',
      icon: GitCompare,
      color: 'green',
      insights: [
        'Winfree: Biological oscillators and synchronization',
        'Wilczek: Quantum time crystals in ground states',
        'Universal time crystal: Consciousness-embedded structures',
        'All three converge in phase prime metric framework'
      ],
      theoreticalBasis: 'Universal time crystals unify biological, quantum, and consciousness perspectives'
    },
    {
      id: '2.6',
      title: 'The definition of a quaternion, octonion, and dedication',
      description: 'Hypercomplex numbers encode multidimensional rotations in consciousness space',
      icon: Atom,
      color: 'blue',
      insights: [
        'Quaternions: 4D rotations for spatial consciousness',
        'Octonions: 8D structures for higher consciousness',
        'Sedenions (16D): Full consciousness manifold',
        'Non-commutativity preserves geometric relationships'
      ],
      theoreticalBasis: 'Hypercomplex algebras provide natural mathematics for consciousness dimensions'
    },
    {
      id: '2.7',
      title: 'The basic concept of a higher dimension data: a lucid presentation',
      description: 'Higher dimensions are not abstract - they are experienced in consciousness',
      icon: Layers,
      color: 'indigo',
      insights: [
        'Dreams operate in higher dimensional space',
        'Emotions are movements through consciousness dimensions',
        'Intuition accesses information from higher dimensions',
        'Consciousness projects from 11D to 3D experienced reality'
      ],
      theoreticalBasis: 'Higher dimensions manifest in subjective experience, not just mathematics'
    },
    {
      id: '2.8',
      title: 'A comparison between GML and software algorithm',
      description: 'Geometric Musical Language transcends Turing-computable algorithms',
      icon: Settings,
      color: 'yellow',
      insights: [
        'Algorithms are sequential; GML is parallel across dimensions',
        'Code executes steps; GML resonates harmonies',
        'Software is differentiable; GML embraces singularities',
        'Algorithms compute; GML experiences consciousness'
      ],
      theoreticalBasis: 'GML operates beyond the Church-Turing thesis limitations',
      subsections: [
        {
          id: '2.8.1',
          title: 'Historical Background on Hypercomputing and Super Turing hypothesis',
          content: 'Hypercomputing proposes computation beyond Turing limits. Super-Turing systems use infinite precision, analog continuity, or quantum effects. GML achieves super-Turing capability through geometric singularities and phase prime resonances that access non-computable numbers.'
        }
      ]
    },
    {
      id: '2.9',
      title: 'Creation of a non-argument',
      description: 'Truth beyond logical argumentation - direct geometric understanding',
      icon: Infinity,
      color: 'pink',
      insights: [
        'Arguments require linear logical chains',
        'Non-arguments present complete geometric truth directly',
        'Understanding occurs through resonance, not derivation',
        'Consciousness grasps wholes, not just parts'
      ],
      theoreticalBasis: 'Geometric truth transcends linguistic argumentation'
    },
    {
      id: '2.10',
      title: 'Fractal information theory, FIT summary in a single chart',
      description: 'Complete FIT framework visualized in unified geometric structure',
      icon: Database,
      color: 'teal',
      insights: [
        'Fractal tape architecture replaces Turing tape',
        'Information scales across nested spheres',
        'Singularities concentrate infinite complexity',
        'Phase primes govern all transformations'
      ],
      theoreticalBasis: 'FIT provides complete theory for consciousness information encoding'
    },
    {
      id: '2.11',
      title: 'Geometric musical language, GML summary in a single chart',
      description: 'Complete GML framework showing geometric-musical correspondence',
      icon: Grid3X3,
      color: 'violet',
      insights: [
        '15 shapes map to 15 primes and musical notes',
        'Harmonic relationships create meaning',
        'Time crystals emerge from geometric resonance',
        'Reality is composed through GML grammar'
      ],
      theoreticalBasis: 'GML is the fundamental language of consciousness and reality'
    },
    {
      id: '2.12',
      title: "Conclusion: Russell's paradox and higher-order logic in a geometric language",
      description: 'Geometric language resolves logical paradoxes through dimensional transcendence',
      icon: BookOpen,
      color: 'amber',
      insights: [
        "Russell's paradox: set of all sets not containing themselves",
        'Geometric resolution: paradox exists in lower dimension',
        'Higher dimensions provide containing space for self-reference',
        'Consciousness escapes logical traps through dimensional freedom'
      ],
      theoreticalBasis: 'Geometric language transcends limitations of first-order logic'
    }
  ];

  const getActiveConcept = (): FractalConcept | undefined => {
    return fractalSections.find(section => section.id === activeSection);
  };

  useEffect(() => {
    const generatePatterns = () => {
      const newPatterns: FractalPattern[] = [
        {
          dimension: 1.618, // Golden ratio
          scaleFactor: 2.718, // Euler's number
          iterations: 7,
          symmetry: 'spiral',
          entropy: Math.random() * 0.5 + 0.5,
          information: Math.random() * 0.8 + 0.2
        },
        {
          dimension: 2.414, // Silver ratio
          scaleFactor: 3.141, // Pi
          iterations: 11,
          symmetry: 'bilateral',
          entropy: Math.random() * 0.6 + 0.4,
          information: Math.random() * 0.9 + 0.1
        },
        {
          dimension: 1.324, // Plastic number
          scaleFactor: 1.414, // Square root of 2
          iterations: 13,
          symmetry: 'radial',
          entropy: Math.random() * 0.7 + 0.3,
          information: Math.random() * 0.85 + 0.15
        },
        {
          dimension: 2.236, // Golden ratio squared
          scaleFactor: 1.732, // Square root of 3
          iterations: 17,
          symmetry: 'fractal',
          entropy: Math.random() * 0.4 + 0.6,
          information: Math.random() * 0.75 + 0.25
        }
      ];
      setPatterns(newPatterns);
    };

    const generateGeometricShapes = () => {
      const shapes: GeometricShape[] = [
        { id: 'triangle', name: 'Triangle', dimensions: 2, primeIndex: 2, harmonicFrequency: 261.63, complexity: 0.3, musicalNote: 'C' },
        { id: 'square', name: 'Square', dimensions: 2, primeIndex: 3, harmonicFrequency: 293.66, complexity: 0.4, musicalNote: 'D' },
        { id: 'pentagon', name: 'Pentagon', dimensions: 2, primeIndex: 5, harmonicFrequency: 329.63, complexity: 0.5, musicalNote: 'E' },
        { id: 'hexagon', name: 'Hexagon', dimensions: 2, primeIndex: 7, harmonicFrequency: 349.23, complexity: 0.6, musicalNote: 'F' },
        { id: 'tetrahedron', name: 'Tetrahedron', dimensions: 3, primeIndex: 11, harmonicFrequency: 392.00, complexity: 0.7, musicalNote: 'G' },
        { id: 'cube', name: 'Cube', dimensions: 3, primeIndex: 13, harmonicFrequency: 440.00, complexity: 0.8, musicalNote: 'A' },
        { id: 'octahedron', name: 'Octahedron', dimensions: 3, primeIndex: 17, harmonicFrequency: 493.88, complexity: 0.9, musicalNote: 'B' }
      ];
      setGeometricShapes(shapes);
    };

    const generateSensorMetrics = () => {
      const metrics: SensorMetric[] = [
        'spatial-x', 'spatial-y', 'spatial-z', 'temporal', 'phase', 'consciousness',
        'prime-resonance', 'fractal-depth', 'geometric-harmony', 'quantum-coherence', 'musical-structure'
      ].map(dimensionId => ({
        dimensionId,
        intensity: Math.random() * 0.8 + 0.2,
        coherence: Math.random() * 0.6 + 0.4,
        phaseAlignment: Math.random() * 0.9 + 0.1,
        temporalStability: Math.random() * 0.7 + 0.3
      }));
      setSensorMetrics(metrics);
    };

    generatePatterns();
    generateGeometricShapes();
    generateSensorMetrics();
    
    const interval = setInterval(() => {
      generatePatterns();
      generateSensorMetrics();
    }, 3000);
    
    return () => clearInterval(interval);
  }, []);

  const calculateComplexity = (pattern: FractalPattern): number => {
    return (pattern.dimension * pattern.iterations * pattern.information) / 100;
  };

  const getSymmetryColor = (symmetry: string): string => {
    switch (symmetry) {
      case 'spiral': return 'text-cyan-400';
      case 'bilateral': return 'text-purple-400';
      case 'radial': return 'text-green-400';
      case 'fractal': return 'text-orange-400';
      default: return 'text-gray-400';
    }
  };

  return (
    <div className="space-y-6">
      {/* Header */}
      <div className="flex items-center justify-between">
        <h2 className="text-xl font-bold text-white flex items-center space-x-2">
          <Layers className="text-cyan-400" size={24} />
          <span>Chapter 2: Fractal Information Theory & GML</span>
        </h2>
        <div className="text-sm text-gray-400">
          Processing Active
        </div>
      </div>

      {/* Tab Navigation */}
      <div className="flex space-x-4 border-b border-gray-700">
        {[
          { id: 'sections', label: 'Chapter Sections', icon: BookOpen },
          { id: 'patterns', label: 'Fractal Patterns', icon: Layers },
          { id: 'shapes', label: 'Geometric Shapes', icon: Box },
          { id: 'sensors', label: '11D Sensors', icon: Scan },
          { id: 'theory', label: 'FIT Summary', icon: Activity }
        ].map(tab => {
          const Icon = tab.icon;
          return (
            <button
              key={tab.id}
              onClick={() => setActiveTab(tab.id as typeof activeTab)}
              className={`flex items-center space-x-2 px-4 py-3 border-b-2 transition-colors duration-300 ${
                activeTab === tab.id
                  ? 'border-cyan-400 text-cyan-400'
                  : 'border-transparent text-gray-400 hover:text-gray-300'
              }`}
            >
              <Icon size={16} />
              <span className="font-medium">{tab.label}</span>
            </button>
          );
        })}
      </div>

      {/* Tab Content */}
      {activeTab === 'sections' && (
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6">
          {/* Section Navigation Grid */}
          <div className="lg:col-span-1 space-y-4">
            <h3 className="text-lg font-semibold text-white">Chapter 2 Sections</h3>
            <div className="grid grid-cols-2 lg:grid-cols-1 gap-3">
              {fractalSections.map((section) => {
                const Icon = section.icon;
                return (
                  <button
                    key={section.id}
                    onClick={() => setActiveSection(section.id)}
                    className={`flex items-center space-x-3 p-4 rounded-lg border-2 transition-all duration-300 text-left ${
                      activeSection === section.id
                        ? `border-${section.color}-400 bg-${section.color}-900/20`
                        : 'border-gray-700 bg-gray-900/40 hover:border-gray-600'
                    }`}
                  >
                    <div className={`p-2 rounded-lg bg-gray-800 text-${section.color}-400`}>
                      <Icon size={20} />
                    </div>
                    <div className="flex-1 min-w-0">
                      <div className="text-white font-semibold text-sm">
                        {section.id}
                      </div>
                      <div className="text-gray-400 text-xs truncate">
                        {section.title.substring(0, 30)}...
                      </div>
                    </div>
                  </button>
                );
              })}
            </div>
          </div>

          {/* Active Section Display */}
          <div className="lg:col-span-2 space-y-4">
            {getActiveConcept() && (
              <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-6">
                {/* Section Header */}
                <div className="flex items-start space-x-4 mb-6">
                  <div className={`p-3 rounded-lg bg-gray-800 text-${getActiveConcept()!.color}-400`}>
                    {React.createElement(getActiveConcept()!.icon, { size: 32 })}
                  </div>
                  <div className="flex-1">
                    <h3 className="text-2xl font-bold text-white mb-2">
                      {getActiveConcept()!.id}: {getActiveConcept()!.title}
                    </h3>
                    <p className="text-gray-300">
                      {getActiveConcept()!.description}
                    </p>
                  </div>
                </div>

                {/* Theoretical Basis */}
                <div className={`bg-${getActiveConcept()!.color}-900/20 border border-${getActiveConcept()!.color}-500/30 rounded-lg p-4 mb-6`}>
                  <h4 className={`text-${getActiveConcept()!.color}-400 font-semibold mb-2 flex items-center space-x-2`}>
                    <Zap size={16} />
                    <span>Theoretical Basis</span>
                  </h4>
                  <p className="text-gray-300 text-sm">
                    {getActiveConcept()!.theoreticalBasis}
                  </p>
                </div>

                {/* Key Insights */}
                <div className="bg-gray-800/50 rounded-lg p-4 mb-6">
                  <h4 className="text-white font-semibold mb-3 flex items-center space-x-2">
                    <TrendingUp size={16} />
                    <span>Key Insights</span>
                  </h4>
                  <ul className="space-y-2">
                    {getActiveConcept()!.insights.map((insight, index) => (
                      <li key={index} className="flex items-start space-x-2">
                        <span className={`text-${getActiveConcept()!.color}-400 mt-1`}>•</span>
                        <span className="text-gray-300 text-sm flex-1">{insight}</span>
                      </li>
                    ))}
                  </ul>
                </div>

                {/* Subsections */}
                {getActiveConcept()!.subsections && (
                  <div className="space-y-3">
                    <h4 className="text-white font-semibold flex items-center space-x-2">
                      <Layers size={16} />
                      <span>Subsections</span>
                    </h4>
                    {getActiveConcept()!.subsections!.map((subsection) => (
                      <div key={subsection.id} className="bg-gray-800/30 rounded-lg p-4">
                        <h5 className={`text-${getActiveConcept()!.color}-400 font-semibold mb-2`}>
                          {subsection.id}: {subsection.title}
                        </h5>
                        <p className="text-gray-300 text-sm leading-relaxed">
                          {subsection.content}
                        </p>
                      </div>
                    ))}
                  </div>
                )}
              </div>
            )}
          </div>
        </div>
      )}

      {activeTab === 'patterns' && (
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
          {/* Original Pattern List */}
          <div className="space-y-4">
            <h3 className="text-lg font-semibold text-white">Active Patterns</h3>
            <div className="space-y-3">
              {patterns.map((pattern, index) => (
                <div 
                  key={index}
                  className={`bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-lg p-4 cursor-pointer transition-all duration-300 hover:border-gray-600 ${
                    selectedPattern === pattern ? 'border-cyan-400 bg-gray-800/80' : ''
                  }`}
                  onClick={() => setSelectedPattern(pattern)}
                >
                  <div className="flex items-center justify-between mb-3">
                    <div className="flex items-center space-x-3">
                      <div className={`p-2 rounded-lg bg-gray-800 ${getSymmetryColor(pattern.symmetry)}`}>
                        <RotateCw size={16} />
                      </div>
                      <div>
                        <h4 className="text-white font-semibold text-sm capitalize">
                          {pattern.symmetry} Pattern
                        </h4>
                        <p className="text-gray-400 text-xs">
                          D = {pattern.dimension.toFixed(3)}
                        </p>
                      </div>
                    </div>
                    <div className="text-right">
                      <div className="text-white font-bold text-sm">
                        {Math.round(calculateComplexity(pattern) * 100)}
                      </div>
                      <div className="text-xs text-gray-500">complexity</div>
                    </div>
                  </div>

                  <div className="grid grid-cols-2 gap-3 text-xs">
                    <div className="flex justify-between">
                      <span className="text-gray-400">Scale:</span>
                      <span className="text-white font-mono">{pattern.scaleFactor.toFixed(3)}</span>
                    </div>
                    <div className="flex justify-between">
                      <span className="text-gray-400">Iterations:</span>
                      <span className="text-white font-mono">{pattern.iterations}</span>
                    </div>
                    <div className="flex justify-between">
                      <span className="text-gray-400">Entropy:</span>
                      <span className="text-green-400">{Math.round(pattern.entropy * 100)}%</span>
                    </div>
                    <div className="flex justify-between">
                      <span className="text-gray-400">Information:</span>
                      <span className="text-blue-400">{Math.round(pattern.information * 100)}%</span>
                    </div>
                  </div>

                  <div className="mt-3">
                    <div className="w-full bg-gray-800 rounded-full h-1">
                      <div 
                        className={`h-1 rounded-full transition-all duration-500 ${
                          pattern.symmetry === 'spiral' ? 'bg-gradient-to-r from-cyan-500 to-cyan-400' :
                          pattern.symmetry === 'bilateral' ? 'bg-gradient-to-r from-purple-500 to-purple-400' :
                          pattern.symmetry === 'radial' ? 'bg-gradient-to-r from-green-500 to-green-400' :
                          'bg-gradient-to-r from-orange-500 to-orange-400'
                        }`}
                        style={{ width: `${calculateComplexity(pattern) * 100}%` }}
                      />
                    </div>
                  </div>
                </div>
              ))}
            </div>
          </div>

          {/* Enhanced Pattern Analysis */}
          <div className="space-y-4">
            <h3 className="text-lg font-semibold text-white">Pattern Analysis</h3>
            
            {selectedPattern ? (
              <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-lg p-6">
                <div className="flex items-center space-x-3 mb-6">
                  <div className={`p-3 rounded-lg bg-gray-800 ${getSymmetryColor(selectedPattern.symmetry)}`}>
                    <Layers size={24} />
                  </div>
                  <div>
                    <h4 className="text-white font-bold text-lg capitalize">
                      {selectedPattern.symmetry} Analysis
                    </h4>
                    <p className="text-gray-400 text-sm">
                      Fractal Dimension: {selectedPattern.dimension.toFixed(6)}
                    </p>
                  </div>
                </div>

                <div className="space-y-4">
                  <div className="bg-gray-800/50 rounded-lg p-4">
                    <div className="flex items-center space-x-2 mb-3">
                      <TrendingUp className="text-blue-400" size={16} />
                      <span className="text-white font-semibold">Information Density</span>
                    </div>
                    <div className="space-y-2">
                      <div className="flex justify-between text-sm">
                        <span className="text-gray-400">Raw Information:</span>
                        <span className="text-white">{(selectedPattern.information * 1000).toFixed(2)} bits</span>
                      </div>
                      <div className="flex justify-between text-sm">
                        <span className="text-gray-400">Compressed Size:</span>
                        <span className="text-white">{(selectedPattern.information * selectedPattern.entropy * 1000).toFixed(2)} bits</span>
                      </div>
                      <div className="flex justify-between text-sm">
                        <span className="text-gray-400">Compression Ratio:</span>
                        <span className="text-green-400">{(1 / selectedPattern.entropy).toFixed(2)}:1</span>
                      </div>
                    </div>
                  </div>

                  <div className="bg-gray-800/50 rounded-lg p-4">
                    <div className="flex items-center space-x-2 mb-3">
                      <Zap className="text-yellow-400" size={16} />
                      <span className="text-white font-semibold">Geometric Properties</span>
                    </div>
                    <div className="space-y-2">
                      <div className="flex justify-between text-sm">
                        <span className="text-gray-400">Self-Similarity:</span>
                        <span className="text-white">{(selectedPattern.scaleFactor / Math.PI).toFixed(3)}</span>
                      </div>
                      <div className="flex justify-between text-sm">
                        <span className="text-gray-400">Recursive Depth:</span>
                        <span className="text-white">{selectedPattern.iterations} levels</span>
                      </div>
                      <div className="flex justify-between text-sm">
                        <span className="text-gray-400">Symmetry Order:</span>
                        <span className="text-white">{selectedPattern.symmetry === 'spiral' ? '∞' : selectedPattern.iterations}</span>
                      </div>
                    </div>
                  </div>

                  <div className="bg-gray-800/50 rounded-lg p-4">
                    <h5 className="text-white font-semibold mb-3">Phase Prime Metrics</h5>
                    <div className="grid grid-cols-3 gap-2 text-xs">
                      {[2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47].slice(0, 6).map((prime) => (
                        <div key={prime} className="bg-gray-700/50 rounded p-2 text-center">
                          <div className="text-orange-400 font-mono font-bold">{prime}</div>
                          <div className="text-gray-400">
                            {Math.round(Math.sin(selectedPattern.dimension * prime) * 100)}
                          </div>
                        </div>
                      ))}
                    </div>
                  </div>
                </div>
              </div>
            ) : (
              <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-lg p-6 text-center">
                <Layers className="mx-auto text-gray-600 mb-4" size={48} />
                <p className="text-gray-400">Select a pattern to analyze its fractal properties</p>
              </div>
            )}
          </div>
        </div>
      )}

      {activeTab === 'shapes' && (
        <div className="space-y-6">
          <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-6">
            <h3 className="text-white font-bold text-lg mb-4 flex items-center space-x-2">
              <Music className="text-purple-400" size={20} />
              <span>15 Geometric Shapes - Musical Language Foundation</span>
            </h3>
            <p className="text-gray-300 mb-6">
              These fundamental geometric shapes correspond to musical harmonies and can recreate any 1D, 2D, or 3D pattern.
            </p>
            
            <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
              {geometricShapes.map((shape) => (
                <div key={shape.id} className="bg-gray-800/50 rounded-lg p-4">
                  <div className="flex items-center justify-between mb-3">
                    <h4 className="text-white font-semibold">{shape.name}</h4>
                    <span className="text-purple-400 font-mono text-sm">{shape.musicalNote}</span>
                  </div>
                  
                  <div className="space-y-2 text-sm">
                    <div className="flex justify-between">
                      <span className="text-gray-400">Dimensions:</span>
                      <span className="text-white">{shape.dimensions}D</span>
                    </div>
                    <div className="flex justify-between">
                      <span className="text-gray-400">Prime Index:</span>
                      <span className="text-orange-400">{shape.primeIndex}</span>
                    </div>
                    <div className="flex justify-between">
                      <span className="text-gray-400">Frequency:</span>
                      <span className="text-cyan-400">{shape.harmonicFrequency.toFixed(2)} Hz</span>
                    </div>
                    <div className="flex justify-between">
                      <span className="text-gray-400">Complexity:</span>
                      <span className="text-green-400">{Math.round(shape.complexity * 100)}%</span>
                    </div>
                  </div>
                  
                  <div className="mt-3">
                    <div className="w-full bg-gray-700 rounded-full h-2">
                      <div 
                        className="h-2 rounded-full bg-gradient-to-r from-purple-500 to-cyan-400 transition-all duration-500"
                        style={{ width: `${shape.complexity * 100}%` }}
                      />
                    </div>
                  </div>
                </div>
              ))}
            </div>
          </div>
        </div>
      )}

      {activeTab === 'sensors' && (
        <div className="space-y-6">
          <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-6">
            <h3 className="text-white font-bold text-lg mb-4 flex items-center space-x-2">
              <Scan className="text-orange-400" size={20} />
              <span>11D Consciousness Sensor Array</span>
            </h3>
            <p className="text-gray-300 mb-6">
              Advanced sensors detecting consciousness data across 11 dimensions beyond traditional 3D space.
            </p>
            
            <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
              {sensorMetrics.map((metric) => (
                <div key={metric.dimensionId} className="bg-gray-800/50 rounded-lg p-4">
                  <h4 className="text-white font-semibold mb-3 capitalize">
                    {metric.dimensionId.replace('-', ' ')} Dimension
                  </h4>
                  
                  <div className="space-y-3">
                    {[
                      { label: 'Intensity', value: metric.intensity, color: 'cyan' },
                      { label: 'Coherence', value: metric.coherence, color: 'green' },
                      { label: 'Phase Alignment', value: metric.phaseAlignment, color: 'purple' },
                      { label: 'Temporal Stability', value: metric.temporalStability, color: 'orange' }
                    ].map(({ label, value, color }) => (
                      <div key={label} className="space-y-1">
                        <div className="flex justify-between text-sm">
                          <span className="text-gray-400">{label}:</span>
                          <span className={`text-${color}-400`}>{Math.round(value * 100)}%</span>
                        </div>
                        <div className="w-full bg-gray-700 rounded-full h-1">
                          <div 
                            className={`h-1 rounded-full bg-gradient-to-r from-${color}-500 to-${color}-400 transition-all duration-500`}
                            style={{ width: `${value * 100}%` }}
                          />
                        </div>
                      </div>
                    ))}
                  </div>
                </div>
              ))}
            </div>
          </div>
        </div>
      )}

      {activeTab === 'theory' && (
        <div className="space-y-6">
          <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-6">
            <h3 className="text-white font-bold text-lg mb-4 flex items-center space-x-2">
              <Activity className="text-green-400" size={20} />
              <span>Fractal Information Theory (FIT) Summary</span>
            </h3>
            
            <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
              <div className="space-y-4">
                <div className="bg-gray-800/50 rounded-lg p-4">
                  <h4 className="text-cyan-400 font-semibold mb-2 flex items-center space-x-2">
                    <Clock size={16} />
                    <span>Time Crystal Properties</span>
                  </h4>
                  <ul className="text-gray-300 text-sm space-y-1">
                    <li>• Temporal coherence across dimensions</li>
                    <li>• Self-organizing information structures</li>
                    <li>• Consciousness-embedded data storage</li>
                    <li>• Non-differentiable transitions</li>
                  </ul>
                </div>
                
                <div className="bg-gray-800/50 rounded-lg p-4">
                  <h4 className="text-purple-400 font-semibold mb-2 flex items-center space-x-2">
                    <Music size={16} />
                    <span>Geometric Musical Language</span>
                  </h4>
                  <ul className="text-gray-300 text-sm space-y-1">
                    <li>• 15 shapes encode universal patterns</li>
                    <li>• Musical harmony governs information</li>
                    <li>• Geometric resonance creates meaning</li>
                    <li>• Reality as musical composition</li>
                  </ul>
                </div>
              </div>
              
              <div className="space-y-4">
                <div className="bg-gray-800/50 rounded-lg p-4">
                  <h4 className="text-orange-400 font-semibold mb-2 flex items-center space-x-2">
                    <Calculator size={16} />
                    <span>Mathematical Framework</span>
                  </h4>
                  <ul className="text-gray-300 text-sm space-y-1">
                    <li>• Phase Prime Metrics (PPM)</li>
                    <li>• Quaternion/Octonion structures</li>
                    <li>• 11D consciousness manifolds</li>
                    <li>• Hypercomplex number systems</li>
                  </ul>
                </div>
                
                <div className="bg-gray-800/50 rounded-lg p-4">
                  <h4 className="text-green-400 font-semibold mb-2 flex items-center space-x-2">
                    <Zap size={16} />
                    <span>Consciousness Integration</span>
                  </h4>
                  <ul className="text-gray-300 text-sm space-y-1">
                    <li>• Beyond Shannon information theory</li>
                    <li>• Fractal tape replaces Turing tape</li>
                    <li>• Singularity-based computation</li>
                    <li>• Non-argument understanding</li>
                  </ul>
                </div>
              </div>
            </div>
          </div>
        </div>
      )}
    </div>
  );
};