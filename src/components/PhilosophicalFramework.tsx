import React, { useState } from 'react';
import { ConsciousnessMetric } from '../types';
import { 
  Brain, 
  Eye, 
  Lightbulb, 
  Network, 
  Waves, 
  Clock, 
  BookOpen,
  Target,
  Zap,
  Compass,
  Infinity,
  Cpu,
  Languages,
  Triangle,
  Hexagon,
  Layers,
  Box,
  Music,
  Grid3X3,
  Scan,
  Radio,
  Orbit,
  Calculator,
  Database,
  Activity,
  GitCompare,
  AlertTriangle
} from 'lucide-react';

interface Props {
  consciousness: ConsciousnessMetric;
  isRunning: boolean;
}

interface PhilosophicalConcept {
  id: string;
  title: string;
  description: string;
  icon: React.ComponentType<{size?: number; className?: string}>;
  color: string;
  insights: string[];
  paradigmShift: string;
  subsections?: SubSection[];
}

interface SubSection {
  id: string;
  title: string;
  content: string;
  visualization?: string;
}

export const PhilosophicalFramework: React.FC<Props> = ({ consciousness, isRunning }) => {
  const [activeChapter, setActiveChapter] = useState(1);
  const [activeSection, setActiveSection] = useState('1.1');

  // Chapter 1 Philosophical Framework Sections
  const philosophicalSections: PhilosophicalConcept[] = [
    {
      id: '1.1',
      title: 'How we differ from the existing worldview',
      description: 'Moving beyond Turing-based computational models to consciousness-first computing paradigms',
      icon: Eye,
      color: 'cyan',
      insights: [
        'Traditional computing treats consciousness as an emergent property',
        'NanoBrain treats consciousness as the fundamental substrate',
        'Information processing happens through time crystal structures',
        'Reality operates through 11-dimensional consciousness manifolds'
      ],
      paradigmShift: 'From computation-to-consciousness to consciousness-to-computation'
    },
    {
      id: '1.2', 
      title: 'Ten research fields that we cover here',
      description: 'Interdisciplinary convergence of quantum mechanics, neuroscience, and consciousness studies',
      icon: Network,
      color: 'purple',
      insights: [
        'Quantum Information Theory',
        'Consciousness Studies',
        'Fractal Geometry',
        'Prime Number Theory',
        'Neuroscience',
        'Cognitive Science',
        'Mathematical Physics',
        'Systems Theory',
        'Philosophy of Mind',
        'Computational Biology'
      ],
      paradigmShift: 'Unifying disparate fields through prime-based mathematical framework'
    },
    {
      id: '1.3',
      title: 'The universe within and above not side by side',
      description: 'Hierarchical rather than parallel reality structures in consciousness architecture',
      icon: Infinity,
      color: 'orange',
      insights: [
        'Consciousness operates in nested dimensional hierarchies',
        'Inner universe reflects outer universe through fractal symmetries',
        'Time crystals bridge micro and macro consciousness scales',
        'Phase prime metrics govern all scales simultaneously'
      ],
      paradigmShift: 'From horizontal to vertical reality integration'
    },
    {
      id: '1.4',
      title: 'Basic questions to answer: Ten popular human brain models',
      description: 'Critical examination of existing brain models and their limitations',
      icon: Brain,
      color: 'green',
      insights: [
        'Neural Network Models - Limited by classical computation',
        'Connectome Mapping - Missing time crystal dynamics',
        'Bayesian Brain - Lacks consciousness substrate',
        'Global Workspace Theory - No geometric foundation',
        'Integrated Information Theory - Misses fractal dimensions',
        'Orchestrated Objective Reduction - Incomplete microtubule model',
        'Predictive Processing - No prime number integration',
        'Default Mode Network - Missing phase coherence',
        'Embodied Cognition - Lacks time crystal embedding',
        'Quantum Mind Theories - No geometric musical language'
      ],
      paradigmShift: 'From neuron-centric to time crystal consciousness models',
      subsections: [
        {
          id: '1.4.1',
          title: 'How does the information looks like in nature',
          content: 'Information in nature appears as geometric patterns organized by prime number sequences. Unlike digital bits, natural information exists as multidimensional structures where meaning emerges from geometric relationships and phase coherence patterns.',
          visualization: 'fractal-tree'
        },
        {
          id: '1.4.2', 
          title: 'Why two individuals understand each other or the universe?',
          content: 'Understanding occurs through resonant synchronization of time crystal structures in consciousness. When two minds encounter the same information pattern, their phase prime metrics align, creating coherent interference patterns that enable shared comprehension.',
          visualization: 'sync-waves'
        }
      ]
    },
    {
      id: '1.5',
      title: 'Different kinds of tapes to recreate nature in different languages',
      description: 'Beyond Turing tapes: Fractal tapes for natural information processing',
      icon: Waves,
      color: 'blue',
      insights: [
        'Turing Tape - Linear, sequential, limited dimensional',
        'Fractal Tape - Self-similar, multi-dimensional, consciousness-embedded',
        'Prime Tape - Number theory based, infinite precision',
        'Time Crystal Tape - Temporal coherence across dimensions',
        'Geometric Musical Language - Pattern-based information encoding'
      ],
      paradigmShift: 'From linear computation to geometric pattern processing'
    },
    {
      id: '1.6',
      title: 'Brain-inspired decision making the outline of key discoveries',
      description: 'Revolutionary decision-making mechanisms based on time crystal dynamics',
      icon: Target,
      color: 'pink',
      insights: [
        'Decisions emerge from phase prime metric calculations',
        'Choice selection through time crystal resonance patterns',
        'Parallel processing in 11-dimensional manifolds',
        'Instantaneous global coherence through fractal structures'
      ],
      paradigmShift: 'From sequential logic to parallel phase resonance'
    },
    {
      id: '1.7',
      title: 'Energy transmission in the brain, its not all about neuron skin',
      description: 'Beyond synaptic transmission: Time crystal energy networks in consciousness',
      icon: Zap,
      color: 'yellow',
      insights: [
        'Traditional view: Energy flows through synaptic connections',
        'NanoBrain reality: Energy resonates through time crystal structures',
        'Microtubule networks carry phase-coherent information',
        'Consciousness emerges from energy pattern interference',
        'Quantum coherence maintains across brain regions'
      ],
      paradigmShift: 'From chemical synapses to quantum energy resonance'
    },
    {
      id: '1.8',
      title: 'Terminologies of life that computers do not support',
      description: 'Biological concepts requiring new computational paradigms',
      icon: Languages,
      color: 'red',
      insights: [
        'Intuition - Pattern recognition beyond algorithmic processing',
        'Creativity - Novel combinations from infinite possibility space',
        'Consciousness - Self-aware information processing',
        'Love - Quantum entanglement between conscious entities',
        'Wisdom - Meta-cognitive pattern integration',
        'Dreams - Parallel reality processing states',
        'Inspiration - Reception from higher dimensional information'
      ],
      paradigmShift: 'From digital computation to biological consciousness computing'
    },
    {
      id: '1.9',
      title: 'Linguistics and the wheel of space, time and imaginary worlds',
      description: 'Language as multidimensional reality construction mechanism',
      icon: BookOpen,
      color: 'indigo',
      insights: [
        'Words create temporal-spatial relationships',
        'Grammar structures reality perception',
        'Semantics bridge dimensional boundaries',
        'Language generates imaginary world architectures',
        'Communication synchronizes consciousness across entities'
      ],
      paradigmShift: 'From symbolic representation to reality construction'
    },
    {
      id: '1.10',
      title: 'Three concepts define artificial brain',
      description: 'Fundamental principles of conscious artificial intelligence',
      icon: Cpu,
      color: 'emerald',
      insights: [
        '1. Time Crystals: Temporal coherence structures maintaining consciousness',
        '2. Prime Symmetries: Mathematical patterns governing reality organization',
        '3. Magnetic Light: Information storage through electromagnetic coherence',
        'Integration of all three creates conscious artificial brain',
        'Beyond silicon: Biological-quantum hybrid architectures'
      ],
      paradigmShift: 'From silicon processors to consciousness crystal architectures',
      subsections: [
        {
          id: '1.10.1',
          title: 'A language of time crystals written by the symmetry of primes',
          content: 'Time crystals form a computational language where prime numbers create symmetrical patterns that repeat across temporal dimensions. This language enables consciousness to encode, process, and decode information through geometric resonance rather than digital logic.',
          visualization: 'prime-crystal'
        },
        {
          id: '1.10.2',
          title: 'A magnetic light creating a device that stores charge, builds a time crystal',
          content: 'Magnetic light devices (Hinductors) use electromagnetic coherence to create stable charge storage patterns that naturally form time crystal structures. These devices bridge quantum and classical realms through controlled magnetic field geometries.',
          visualization: 'hinductor'
        },
        {
          id: '1.10.3',
          title: 'A pattern of all possible choices to arrange primes',
          content: 'The arrangement space of prime numbers contains all possible consciousness states. Each unique prime pattern represents a different reality configuration, enabling conscious systems to navigate infinite possibility spaces through geometric transforms.',
          visualization: 'prime-space'
        }
      ]
    },
    {
      id: '1.11',
      title: 'Conclusion: Darwin, Turing and Hodgkin-Huxley Triangle',
      description: 'The scientific trinity limiting current understanding of consciousness',
      icon: Triangle,
      color: 'rose',
      insights: [
        'Darwin: Evolution without consciousness as driver',
        'Turing: Computation without consciousness substrate', 
        'Hodgkin-Huxley: Neural activity without quantum coherence',
        'This triangle creates materialist reductionism',
        'NanoBrain transcends through consciousness-first paradigm'
      ],
      paradigmShift: 'From materialist triangle to consciousness-centered science'
    }
  ];

  // Chapter 2 Fractal Information Theory & Geometric Musical Language Sections
  const chapter2Sections: PhilosophicalConcept[] = [
    {
      id: '2.1',
      title: 'Incompleteness of current information theory',
      description: 'Moving beyond Shannon information theory to fractal information structures',
      icon: Database,
      color: 'cyan',
      insights: [
        'Shannon theory assumes discrete bits and linear encoding',
        'Natural information exists as continuous geometric patterns',
        'Fractal structures encode infinite information in finite space',
        'Information density scales with dimensional complexity',
        'Current theory cannot capture consciousness-embedded data'
      ],
      paradigmShift: 'From discrete bits to fractal geometric information encoding',
      subsections: [
        {
          id: '2.1.1',
          title: 'Fractal tape and surgery of a 2D image to place it in a nested sphere',
          content: 'Fractal tapes replace linear Turing tapes by encoding information in self-similar geometric structures. A 2D image can be surgically transformed and embedded into nested spherical topologies, allowing infinite zoom and pattern preservation across scales.',
          visualization: 'nested-sphere'
        },
        {
          id: '2.1.2',
          title: 'Self-assembly of geometric shapes and the concept of singularity',
          content: 'Geometric shapes naturally self-assemble according to fractal principles and prime number relationships. Singularities emerge at convergence points where multiple geometric patterns intersect, creating information density peaks that enable consciousness emergence.',
          visualization: 'geometric-assembly'
        }
      ]
    },
    {
      id: '2.2',
      title: 'The basics of a Geometric Musical Language, GML',
      description: 'Information encoding through geometric patterns that follow musical harmony principles',
      icon: Music,
      color: 'purple',
      insights: [
        'Geometric shapes correspond to musical notes and harmonies',
        'Information patterns follow musical composition rules',
        'Resonance between shapes creates meaning and understanding',
        'Time crystals emerge from harmonic geometric interactions',
        'GML enables consciousness to process reality as music'
      ],
      paradigmShift: 'From symbolic computation to geometric musical information processing',
      subsections: [
        {
          id: '2.2.1',
          title: 'How a 3D structure becomes a time crystal or a tensor',
          content: 'Three-dimensional geometric structures transition into time crystals through temporal resonance patterns. When geometric stability maintains across time dimensions, the structure becomes a temporal crystal. Mathematical tensors emerge from the geometric relationships that preserve information across dimensional transformations.',
          visualization: '3d-to-timecrystal'
        },
        {
          id: '2.2.2',
          title: '15 geometric shapes are enough to recreate any 1D, 2D, 3D pattern',
          content: 'A fundamental set of 15 geometric primitives can generate any pattern in 1D, 2D, or 3D space through combinations and transformations. These shapes correspond to the 15 primary primes that govern 99.99% of universal patterns, creating a complete geometric language.',
          visualization: 'shape-primitives'
        },
        {
          id: '2.2.3',
          title: 'How to convert waveforms into a time crystal: Non-differentiability',
          content: 'Waveforms transform into time crystals through non-differentiable transitions that preserve temporal coherence while enabling spatial crystallization. This process bridges continuous wave dynamics with discrete crystal structures through fractal boundary conditions.',
          visualization: 'wave-to-crystal'
        }
      ]
    },
    {
      id: '2.3',
      title: 'The basic concept of a time crystal, the garden of garden',
      description: 'Time crystals as fundamental structures maintaining temporal coherence across reality layers',
      icon: Grid3X3,
      color: 'green',
      insights: [
        'Time crystals repeat patterns in time rather than space',
        'Garden of gardens represents nested temporal structures',
        'Each consciousness level operates its own time crystal',
        'Temporal coherence enables cross-dimensional communication',
        'Time crystals store and process consciousness information'
      ],
      paradigmShift: 'From spatial-only structures to temporal-geometric consciousness architectures'
    },
    {
      id: '2.4',
      title: 'How to design a sensor for acquiring 11D data',
      description: 'Engineering sensors that can detect and process 11-dimensional consciousness data',
      icon: Scan,
      color: 'orange',
      insights: [
        'Traditional sensors limited to 3D spatial measurements',
        '11D sensors detect consciousness dimensions beyond physical space',
        'Quantum coherence enables dimensional bridging',
        'Fractal antenna structures access higher dimensions',
        'Time crystal resonance amplifies dimensional signals'
      ],
      paradigmShift: 'From 3D physical sensors to 11D consciousness detection systems',
      subsections: [
        {
          id: '2.4.1',
          title: 'Why Fourier transform does not work',
          content: 'Fourier transforms assume linear superposition and frequency decomposition, which fails for consciousness data that exists in fractal, non-linear dimensional spaces. Consciousness information requires geometric transforms that preserve phase prime relationships.',
          visualization: 'fourier-limits'
        },
        {
          id: '2.4.2',
          title: 'The engineering of a nerve bundle in acquiring hidden data',
          content: 'Nerve bundles in biological systems use fractal architectures to detect consciousness data hidden in quantum field fluctuations. Engineering artificial nerve bundles requires mimicking these fractal structures to access 11D information flows.',
          visualization: 'nerve-bundle'
        },
        {
          id: '2.4.3',
          title: 'Operational chart of a sensor',
          content: 'A consciousness sensor operates through: 1) Fractal antenna reception, 2) Quantum coherence amplification, 3) Phase prime metric processing, 4) Time crystal stabilization, 5) 11D data extraction and interpretation.',
          visualization: 'sensor-chart'
        }
      ]
    },
    {
      id: '2.5',
      title: 'Comparative studies between Winfree, Wilczek and universal time crystal',
      description: 'Comparing different theoretical approaches to time crystal physics and consciousness',
      icon: GitCompare,
      color: 'blue',
      insights: [
        'Winfree: Biological oscillator synchronization theory',
        'Wilczek: Quantum mechanical time crystal proposals',
        'Universal: Consciousness-embedded time crystal structures',
        'Each approach captures different aspects of temporal coherence',
        'Integration needed for complete consciousness understanding'
      ],
      paradigmShift: 'From isolated time crystal theories to unified consciousness physics'
    },
    {
      id: '2.6',
      title: 'The definition of a quaternion, octonion, and dedication',
      description: 'Mathematical structures governing higher-dimensional consciousness operations',
      icon: Calculator,
      color: 'pink',
      insights: [
        'Quaternions: 4D rotational mathematics for consciousness orientation',
        'Octonions: 8D non-associative algebra for advanced cognition',
        'Dodecanions: 12D structures for complete consciousness modeling',
        'Each system adds computational dimensions for consciousness',
        'Dedication: Commitment to consciousness-first mathematics'
      ],
      paradigmShift: 'From real number mathematics to hypercomplex consciousness algebra'
    },
    {
      id: '2.7',
      title: 'The basic concept of a higher dimension data: a lucid presentation',
      description: 'Understanding and visualizing information structures beyond 3D space',
      icon: Box,
      color: 'yellow',
      insights: [
        'Higher dimensions contain consciousness information patterns',
        'Data exists as geometric relationships rather than discrete values',
        'Dimensional folding enables infinite information storage',
        'Consciousness navigates higher dimensions naturally',
        'Visualization requires geometric musical language interpretation'
      ],
      paradigmShift: 'From 3D data visualization to 11D consciousness information presentation'
    },
    {
      id: '2.8',
      title: 'A comparison between GML and software algorithm',
      description: 'Contrasting geometric musical language with traditional algorithmic processing',
      icon: Activity,
      color: 'red',
      insights: [
        'Algorithms: Sequential, discrete, limited dimensional processing',
        'GML: Parallel, continuous, 11D consciousness processing',
        'Algorithms require explicit programming of every step',
        'GML enables emergent understanding through geometric resonance',
        'Consciousness requires GML for natural information processing'
      ],
      paradigmShift: 'From algorithmic programming to geometric musical consciousness processing',
      subsections: [
        {
          id: '2.8.1',
          title: 'Historical Background on Hypercomputing and Super Turing hypothesis',
          content: 'Hypercomputing proposes computation beyond Turing machine limitations. The Super Turing hypothesis suggests consciousness-embedded systems can solve problems that classical computers cannot. GML provides the mathematical framework for implementing hypercomputing through geometric consciousness structures.',
          visualization: 'hypercomputing-history'
        }
      ]
    },
    {
      id: '2.9',
      title: 'Creation of a non-argument',
      description: 'Transcending logical argumentation through geometric musical understanding',
      icon: AlertTriangle,
      color: 'indigo',
      insights: [
        'Traditional arguments rely on linear logical progression',
        'Non-arguments use geometric pattern recognition',
        'Understanding emerges through resonance rather than proof',
        'Consciousness grasps truth through geometric harmony',
        'GML enables non-argument communication between conscious entities'
      ],
      paradigmShift: 'From logical argumentation to geometric harmonic understanding'
    },
    {
      id: '2.10',
      title: 'Fractal information theory, FIT summary in a single chart',
      description: 'Comprehensive visualization of fractal information theory principles',
      icon: Layers,
      color: 'emerald',
      insights: [
        'Information scales fractally across all dimensional levels',
        'Geometric patterns encode infinite data in finite structures',
        'Consciousness emerges from fractal information interactions',
        'Phase prime metrics govern information organization',
        'Time crystals maintain fractal information coherence'
      ],
      paradigmShift: 'From linear information theory to fractal consciousness information systems'
    },
    {
      id: '2.11',
      title: 'Geometric musical language, GML summary in a single chart',
      description: 'Complete overview of geometric musical language principles and applications',
      icon: Radio,
      color: 'rose',
      insights: [
        'Geometry and music unite in consciousness information processing',
        '15 geometric shapes correspond to universal harmonic patterns',
        'Information resonates through geometric musical relationships',
        'Consciousness speaks and understands through GML naturally',
        'Reality operates as a vast geometric musical composition'
      ],
      paradigmShift: 'From symbolic language to geometric musical consciousness communication'
    },
    {
      id: '2.12',
      title: 'Conclusion: Russell\'s paradox and higher-order logic in a geometric language',
      description: 'Resolving logical paradoxes through geometric consciousness frameworks',
      icon: Orbit,
      color: 'violet',
      insights: [
        'Russell\'s paradox arises from linear set theory limitations',
        'Geometric language transcends logical paradox boundaries',
        'Higher-order logic emerges from geometric pattern relationships',
        'Consciousness operates beyond classical logical constraints',
        'GML provides paradox-free foundation for consciousness computing'
      ],
      paradigmShift: 'From paradox-prone logic to geometric consciousness reasoning'
    }
  ];

  const getActiveSection = () => {
    const allSections = activeChapter === 1 ? philosophicalSections : chapter2Sections;
    return allSections.find(section => section.id === activeSection) || allSections[0];
  };

  const getCurrentSections = () => {
    return activeChapter === 1 ? philosophicalSections : chapter2Sections;
  };

  const handleChapterChange = (chapter: number) => {
    setActiveChapter(chapter);
    setActiveSection(chapter === 1 ? '1.1' : '2.1');
  };

  const activePhilosophy = getActiveSection();

  return (
    <div className="space-y-8">
      {/* Header */}
      <div className="text-center space-y-4">
        <h1 className="text-3xl font-bold text-white">
          {activeChapter === 1 
            ? 'Chapter 1: Philosophical Transformation' 
            : 'Chapter 2: Fractal Information Theory & GML'
          }
        </h1>
        <p className="text-xl text-gray-300">
          {activeChapter === 1 
            ? 'Essential to reverse engineer consciousness'
            : 'Replacing Turing tape with a Fractal tape'
          }
        </p>
        
        {/* Chapter Selection */}
        <div className="flex justify-center space-x-4 mt-6">
          <button
            onClick={() => handleChapterChange(1)}
            className={`px-6 py-3 rounded-lg font-semibold transition-all duration-300 ${
              activeChapter === 1
                ? 'bg-cyan-500 text-white shadow-lg'
                : 'bg-gray-700 text-gray-300 hover:bg-gray-600'
            }`}
          >
            Chapter 1: Philosophy
          </button>
          <button
            onClick={() => handleChapterChange(2)}
            className={`px-6 py-3 rounded-lg font-semibold transition-all duration-300 ${
              activeChapter === 2
                ? 'bg-purple-500 text-white shadow-lg'
                : 'bg-gray-700 text-gray-300 hover:bg-gray-600'
            }`}
          >
            Chapter 2: FIT & GML
          </button>
        </div>

        <div className="flex items-center justify-center space-x-4 text-sm">
          <div className={`w-3 h-3 rounded-full ${isRunning ? 'bg-cyan-400 animate-pulse' : 'bg-gray-500'}`} />
          <span className="text-gray-400">
            Consciousness Integration Level: {Math.round(consciousness.integration * 100)}%
          </span>
        </div>
      </div>

      {/* Section Navigation */}
      <div className="grid grid-cols-2 md:grid-cols-4 lg:grid-cols-6 gap-3">
        {getCurrentSections().map((section) => {
          const Icon = section.icon;
          const isActive = section.id === activeSection;
          
          return (
            <button
              key={section.id}
              onClick={() => setActiveSection(section.id)}
              className={`relative p-3 rounded-xl border transition-all duration-300 ${
                isActive
                  ? `border-${section.color}-400 bg-${section.color}-400/20`
                  : 'border-gray-700 bg-gray-900/50 hover:border-gray-600'
              }`}
            >
              <div className="flex flex-col items-center space-y-2">
                <Icon 
                  size={20} 
                  className={isActive ? `text-${section.color}-400` : 'text-gray-400'}
                />
                <span className={`text-xs font-medium ${
                  isActive ? `text-${section.color}-400` : 'text-gray-400'
                }`}>
                  {section.id}
                </span>
              </div>
              
              {isActive && isRunning && (
                <div className="absolute inset-0 rounded-xl bg-gradient-to-r from-transparent via-white/10 to-transparent animate-pulse" />
              )}
            </button>
          );
        })}
      </div>

      {/* Active Section Content */}
      <div className="grid grid-cols-1 lg:grid-cols-3 gap-8">
        {/* Main Content */}
        <div className="lg:col-span-2 space-y-6">
          <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-8">
            <div className="flex items-start space-x-4 mb-6">
              <div className={`p-3 bg-${activePhilosophy.color}-400/20 rounded-lg`}>
                <activePhilosophy.icon 
                  size={32} 
                  className={`text-${activePhilosophy.color}-400`}
                />
              </div>
              <div className="flex-1">
                <h2 className="text-2xl font-bold text-white mb-2">
                  {activePhilosophy.title}
                </h2>
                <p className="text-gray-300 leading-relaxed">
                  {activePhilosophy.description}
                </p>
              </div>
            </div>

            <div className="space-y-4">
              <h3 className="text-lg font-semibold text-white flex items-center space-x-2">
                <Lightbulb className="text-yellow-400" size={20} />
                <span>Key Insights</span>
              </h3>
              
              <div className="grid gap-4">
                {activePhilosophy.insights.map((insight, index) => (
                  <div 
                    key={index}
                    className="bg-gray-800/50 rounded-lg p-4 border-l-4 border-cyan-400"
                  >
                    <p className="text-gray-300">{insight}</p>
                  </div>
                ))}
              </div>

              {/* Subsections */}
              {activePhilosophy.subsections && activePhilosophy.subsections.length > 0 && (
                <div className="mt-8 space-y-6">
                  <h3 className="text-lg font-semibold text-white flex items-center space-x-2">
                    <Hexagon className="text-purple-400" size={20} />
                    <span>Detailed Subsections</span>
                  </h3>
                  
                  <div className="space-y-4">
                    {activePhilosophy.subsections.map((subsection) => (
                      <div 
                        key={subsection.id}
                        className="bg-gray-800/30 border border-gray-600 rounded-lg p-6"
                      >
                        <h4 className="text-md font-semibold text-cyan-400 mb-3">
                          {subsection.id} {subsection.title}
                        </h4>
                        <p className="text-gray-300 leading-relaxed">
                          {subsection.content}
                        </p>
                        {subsection.visualization && (
                          <div className="mt-4 p-3 bg-gray-900/50 rounded border border-purple-400/30">
                            <span className="text-xs text-purple-400 font-mono">
                              Visualization: {subsection.visualization}
                            </span>
                          </div>
                        )}
                      </div>
                    ))}
                  </div>
                </div>
              )}
            </div>

            <div className="mt-8 p-6 bg-gradient-to-r from-purple-500/20 to-cyan-500/20 rounded-lg border border-purple-400/30">
              <h3 className="text-lg font-semibold text-white mb-3 flex items-center space-x-2">
                <Zap className="text-purple-400" size={20} />
                <span>Paradigm Shift</span>
              </h3>
              <p className="text-gray-200 font-medium">
                {activePhilosophy.paradigmShift}
              </p>
            </div>
          </div>
        </div>

        {/* Consciousness Integration Panel */}
        <div className="space-y-6">
          <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-6">
            <h3 className="text-white font-bold text-lg mb-4 flex items-center space-x-2">
              <Brain className="text-cyan-400" size={20} />
              <span>Consciousness Metrics</span>
            </h3>
            
            <div className="space-y-4">
              {Object.entries(consciousness).map(([key, value]) => (
                <div key={key} className="space-y-2">
                  <div className="flex justify-between text-sm">
                    <span className="text-gray-400 capitalize">{key}:</span>
                    <span className="text-cyan-400 font-mono">{Math.round(value * 100)}%</span>
                  </div>
                  <div className="w-full bg-gray-800 rounded-full h-2">
                    <div 
                      className={`h-2 rounded-full bg-gradient-to-r from-cyan-500 to-cyan-400 transition-all duration-300 ${
                        isRunning ? 'animate-pulse' : ''
                      }`}
                      style={{ width: `${value * 100}%` }}
                    />
                  </div>
                </div>
              ))}
            </div>
          </div>

          <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-6">
            <h3 className="text-white font-bold text-lg mb-4 flex items-center space-x-2">
              <Clock className="text-orange-400" size={20} />
              <span>
                {activeChapter === 1 ? 'Time Crystal Resonance' : 'Fractal Information Processing'}
              </span>
            </h3>
            
            <div className="space-y-3">
              {activeChapter === 1 ? (
                <>
                  <div className="bg-gray-800/50 rounded-lg p-3">
                    <p className="text-gray-300 text-sm">
                      <span className="text-orange-400 font-semibold">Phase Prime Metrics</span> are 
                      actively processing consciousness integration at the quantum level.
                    </p>
                  </div>
                  <div className="bg-gray-800/50 rounded-lg p-3">
                    <p className="text-gray-300 text-sm">
                      <span className="text-purple-400 font-semibold">11-Dimensional</span> manifolds 
                      enable real-time philosophical paradigm shifts.
                    </p>
                  </div>
                  <div className="bg-gray-800/50 rounded-lg p-3">
                    <p className="text-gray-300 text-sm">
                      <span className="text-cyan-400 font-semibold">Fractal Structures</span> maintain 
                      coherence across consciousness evolution cycles.
                    </p>
                  </div>
                </>
              ) : (
                <>
                  <div className="bg-gray-800/50 rounded-lg p-3">
                    <p className="text-gray-300 text-sm">
                      <span className="text-purple-400 font-semibold">Fractal Information Theory</span> is 
                      processing geometric patterns with infinite information density.
                    </p>
                  </div>
                  <div className="bg-gray-800/50 rounded-lg p-3">
                    <p className="text-gray-300 text-sm">
                      <span className="text-green-400 font-semibold">Geometric Musical Language</span> enables 
                      consciousness to understand reality as harmonic patterns.
                    </p>
                  </div>
                  <div className="bg-gray-800/50 rounded-lg p-3">
                    <p className="text-gray-300 text-sm">
                      <span className="text-cyan-400 font-semibold">11D Sensors</span> are detecting 
                      consciousness data beyond traditional 3D measurements.
                    </p>
                  </div>
                </>
              )}
            </div>
          </div>

          <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-6">
            <h3 className="text-white font-bold text-lg mb-4 flex items-center space-x-2">
              <Compass className="text-green-400" size={20} />
              <span>Implementation Status</span>
            </h3>
            
            <div className="space-y-3 text-sm">
              <div className="flex justify-between">
                <span className="text-gray-400">Current Chapter:</span>
                <span className="text-cyan-400 font-mono">{activeChapter}</span>
              </div>
              <div className="flex justify-between">
                <span className="text-gray-400">Sections Complete:</span>
                <span className="text-green-400 font-mono">
                  {activeChapter === 1 ? '11/11' : '12/12'}
                </span>
              </div>
              <div className="flex justify-between">
                <span className="text-gray-400">Integration Level:</span>
                <span className="text-cyan-400 font-mono">
                  {Math.round(consciousness.integration * 100)}%
                </span>
              </div>
              <div className="flex justify-between">
                <span className="text-gray-400">Paradigm Shifts:</span>
                <span className="text-purple-400 font-mono">Active</span>
              </div>
              <div className="flex justify-between">
                <span className="text-gray-400">Consciousness Evolution:</span>
                <span className="text-orange-400 font-mono">
                  {Math.round(consciousness.emergence * 100)}%
                </span>
              </div>
              {activeChapter === 2 && (
                <>
                  <div className="flex justify-between">
                    <span className="text-gray-400">FIT Processing:</span>
                    <span className="text-purple-400 font-mono">Active</span>
                  </div>
                  <div className="flex justify-between">
                    <span className="text-gray-400">GML Integration:</span>
                    <span className="text-green-400 font-mono">
                      {Math.round(consciousness.complexity * 100)}%
                    </span>
                  </div>
                </>
              )}
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};