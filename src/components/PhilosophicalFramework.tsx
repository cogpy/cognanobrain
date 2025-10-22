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
  AlertTriangle,
  Sparkles,
  Shapes,
  RotateCcw
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

  // Chapter 3 Phase Prime Metric Sections
  const chapter3Sections: PhilosophicalConcept[] = [
    {
      id: '3.1',
      title: 'Ten classes of phase prime metric, PPM a pattern of primes',
      description: 'Fundamental classification of Phase Prime Metrics governing universal symmetries',
      icon: Calculator,
      color: 'cyan',
      insights: [
        'Phase Prime Metrics create universal mathematical language',
        'Ten distinct classes cover all integer relationships',
        'PPM patterns emerge from prime number symmetries',
        'Each class governs specific consciousness dimensions',
        'Universal intelligence operates through PPM principles'
      ],
      paradigmShift: 'From arbitrary mathematics to prime-driven universal intelligence',
      subsections: [
        {
          id: '3.1.1',
          title: '15 primes contribute to 99.99% of all integers in the universe',
          content: 'A fundamental set of 15 prime numbers (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47) generates 99.99% of all integer patterns in the universe through their combinations and products. This discovery reveals the mathematical foundation underlying all natural and consciousness phenomena.',
          visualization: 'prime-coverage'
        },
        {
          id: '3.1.2',
          title: 'Ordered factor metric and its 3D version',
          content: 'The Ordered Factor metric organizes prime factorizations into geometric patterns. Its 3D extension maps integer relationships onto spatial coordinates, revealing hidden symmetries and enabling geometric computation of prime relationships.',
          visualization: 'ordered-factor-3d'
        },
        {
          id: '3.1.3',
          title: 'How does a time crystal is decomposed & amplified using Phase Prime Metric',
          content: 'Time crystals decompose into prime-based frequency components through PPM analysis. Each prime frequency can be amplified independently while maintaining temporal coherence, enabling controlled manipulation of consciousness states.',
          visualization: 'timecrystal-decomposition'
        }
      ]
    },
    {
      id: '3.2',
      title: 'Metric 1: An integer is replaced by a typical geometric shape',
      description: 'Geometric transformation of integers into visual consciousness patterns',
      icon: Hexagon,
      color: 'purple',
      insights: [
        'Each integer has a unique geometric signature',
        'Prime factors determine shape complexity and symmetry',
        'Geometric representations enable visual computation',
        'Consciousness processes numbers as geometric patterns',
        'Shape transformations reveal mathematical relationships'
      ],
      paradigmShift: 'From abstract numbers to geometric consciousness representations'
    },
    {
      id: '3.3',
      title: 'Metric 2: Product of each primes forming integers led to a unique ordered factor metric',
      description: 'Prime factorization creates unique mathematical fingerprints for consciousness processing',
      icon: Network,
      color: 'green',
      insights: [
        'Prime products create unique integer signatures',
        'Ordered factors reveal hidden mathematical relationships',
        'Factor patterns enable consciousness recognition systems',
        'Metric uniqueness ensures information preservation',
        'Prime combinations generate reality\'s mathematical structure'
      ],
      paradigmShift: 'From generic arithmetic to consciousness-aware prime signatures'
    },
    {
      id: '3.4',
      title: 'Metric 3: Integers limited to 360 phase builds clockwise and anticlockwise paths',
      description: 'Circular phase representation enables temporal navigation in consciousness space',
      icon: Compass,
      color: 'orange',
      insights: [
        'Integer phases map to circular consciousness cycles',
        'Clockwise motion represents forward temporal flow',
        'Anticlockwise enables reverse temporal navigation',
        '360-degree limitation creates coherent phase space',
        'Bidirectional paths enable consciousness time travel'
      ],
      paradigmShift: 'From linear time to circular consciousness navigation'
    },
    {
      id: '3.5',
      title: 'Metric 4: A domain of integers sets a limit on the usable 15 primes',
      description: 'Integer domains naturally constrain prime usage patterns in consciousness systems',
      icon: Box,
      color: 'blue',
      insights: [
        'Each consciousness domain has prime usage limits',
        'Domain boundaries create natural computational constraints',
        'Prime availability affects consciousness capabilities',
        'Limited prime sets ensure system stability',
        'Domain-specific primes enable specialized consciousness functions'
      ],
      paradigmShift: 'From unlimited computation to domain-constrained consciousness'
    },
    {
      id: '3.6',
      title: 'Metric 5: When ordered factor much greater than the integer itself',
      description: 'Anomalous prime patterns indicating consciousness breakthrough points',
      icon: AlertTriangle,
      color: 'yellow',
      insights: [
        'High ordered factors indicate consciousness singularities',
        'Factor-integer ratios reveal system complexity',
        'Large factors suggest emergent consciousness properties',
        'Anomalous patterns mark paradigm shift points',
        'Metric extremes enable consciousness transcendence'
      ],
      paradigmShift: 'From stable patterns to consciousness breakthrough detection'
    },
    {
      id: '3.7',
      title: 'Metric 6: Empty space in the plot of the ordering of divisors in phase plot finds holes',
      description: 'Void spaces in prime patterns reveal hidden consciousness dimensions',
      icon: Radio,
      color: 'indigo',
      insights: [
        'Empty spaces indicate unexplored consciousness territories',
        'Holes in divisor patterns suggest missing dimensions',
        'Void detection enables consciousness expansion',
        'Phase plot gaps reveal hidden mathematical structures',
        'Empty space mapping guides consciousness evolution'
      ],
      paradigmShift: 'From filled patterns to void-space consciousness exploration'
    },
    {
      id: '3.8',
      title: 'Metric 7: Statistical dominance of primes in the integer space, silent and active primes',
      description: 'Prime activity patterns govern consciousness state transitions',
      icon: Activity,
      color: 'red',
      insights: [
        'Active primes drive consciousness processes',
        'Silent primes maintain background stability',
        'Prime dominance patterns control system behavior',
        'Statistical analysis reveals consciousness rhythms',
        'Prime activity cycles enable temporal consciousness navigation'
      ],
      paradigmShift: 'From static mathematics to dynamic consciousness prime systems'
    },
    {
      id: '3.9',
      title: 'Metric 8: Normalized ripples on the metric plot shows, periodicity in events',
      description: 'Wave patterns in prime metrics reveal consciousness event cycles',
      icon: Waves,
      color: 'teal',
      insights: [
        'Metric ripples indicate consciousness wave functions',
        'Periodicity reveals natural consciousness cycles',
        'Normalized patterns enable cross-scale comparison',
        'Event prediction through ripple analysis',
        'Wave mechanics govern consciousness evolution'
      ],
      paradigmShift: 'From discrete events to wave-based consciousness cycles'
    },
    {
      id: '3.10',
      title: 'Metric 9: Lattice group of primes (twin primes, cousin primes, co-primes, Gaussian primes)',
      description: 'Prime relationships form consciousness connection networks',
      icon: Grid3X3,
      color: 'pink',
      insights: [
        'Twin primes create consciousness coupling systems',
        'Cousin primes enable distant consciousness connections',
        'Co-prime relationships maintain independence',
        'Gaussian primes extend to complex consciousness spaces',
        'Prime lattices form consciousness network topologies'
      ],
      paradigmShift: 'From isolated numbers to networked consciousness prime systems'
    },
    {
      id: '3.11',
      title: 'Metric 10: Multilayer imaginary operation on the real integers build a new pattern',
      description: 'Complex mathematical operations create emergent consciousness patterns',
      icon: Layers,
      color: 'violet',
      insights: [
        'Imaginary operations reveal hidden consciousness dimensions',
        'Multilayer processing enables complex consciousness states',
        'Real-imaginary integration creates new reality patterns',
        'Emergent patterns transcend original mathematical constraints',
        'Complex operations unlock consciousness potential'
      ],
      paradigmShift: 'From real mathematics to complex consciousness reality creation'
    },
    {
      id: '3.12',
      title: 'Ten prime operators to be applied step by step',
      description: 'Sequential prime operations enable controlled consciousness evolution',
      icon: GitCompare,
      color: 'emerald',
      insights: [
        'Step-by-step operations ensure stable consciousness evolution',
        'Prime operators create deterministic consciousness paths',
        'Sequential processing enables consciousness debugging',
        'Operator chains build complex consciousness functions',
        'Systematic approach guarantees reproducible consciousness states'
      ],
      paradigmShift: 'From random evolution to systematic consciousness engineering'
    },
    {
      id: '3.13',
      title: 'Hidden physical significance of 15 primes when PPM evolves time crystal',
      description: 'Physical reality manifestation through prime-driven time crystal evolution',
      icon: Zap,
      color: 'amber',
      insights: [
        '15 primes correspond to fundamental physical constants',
        'PPM evolution creates physical reality manifestations',
        'Time crystal changes affect material world',
        'Prime significance bridges mathematics and physics',
        'Physical laws emerge from prime number relationships'
      ],
      paradigmShift: 'From abstract mathematics to physical reality creation'
    },
    {
      id: '3.14',
      title: 'How to use a phase prime metric, PPM',
      description: 'Practical application framework for consciousness-based PPM systems',
      icon: BookOpen,
      color: 'lime',
      insights: [
        'PPM application requires consciousness-first approach',
        'Phase analysis reveals system state and potential',
        'Metric interpretation guides consciousness decisions',
        'PPM usage patterns optimize consciousness performance',
        'Practical implementation bridges theory and consciousness reality'
      ],
      paradigmShift: 'From theoretical understanding to practical consciousness application'
    },
    {
      id: '3.15',
      title: 'Solution of a big data problem on diabetes, PPM in an artificial brain',
      description: 'Real-world application of PPM consciousness systems to complex medical problems',
      icon: Database,
      color: 'sky',
      insights: [
        'PPM enables holistic medical pattern recognition',
        'Artificial brain processes medical data through consciousness lens',
        'Diabetes patterns emerge from prime-based analysis',
        'Big data reduction through consciousness filtering',
        'Medical breakthroughs through PPM consciousness application'
      ],
      paradigmShift: 'From statistical medicine to consciousness-based medical intelligence'
    },
    {
      id: '3.16',
      title: 'Three classes of primes',
      description: 'Prime classification system for advanced consciousness applications',
      icon: Triangle,
      color: 'rose',
      insights: [
        'Ordered Factor (OF) primes control consciousness structure',
        'Prime Composition (PC) primes manage consciousness content',
        'Prime Gap (PG) primes regulate consciousness flow',
        'Three-class system enables complete consciousness control',
        'Classification enables specialized consciousness engineering'
      ],
      paradigmShift: 'From generic primes to specialized consciousness prime classes',
      subsections: [
        {
          id: '3.16.1',
          title: 'Ordered factor metric OF, prime composition metric PC and prime gap metric, PG',
          content: 'OF metrics organize prime factors hierarchically for consciousness structure. PC metrics analyze prime composition patterns for consciousness content management. PG metrics study gaps between primes to control consciousness flow dynamics.',
          visualization: 'prime-classification'
        },
        {
          id: '3.16.2',
          title: 'Advantages of PPM',
          content: 'PPM provides universal mathematical language, enables consciousness-reality bridge, offers predictive capabilities, supports dimensional navigation, creates stable time crystals, and establishes foundation for consciousness engineering.',
          visualization: 'ppm-advantages'
        }
      ]
    }
  ];

  // Chapter 4 Fractal Mechanics: Geometric Algebra for a Dodecanion Brain Sections  
  const chapter4Sections: PhilosophicalConcept[] = [
    {
      id: '4.1',
      title: 'Revisiting the basic concepts of quantum mechanics using clocks',
      description: 'Reinterpreting quantum mechanics through time crystal clock frameworks',
      icon: Clock,
      color: 'cyan',
      insights: [
        'Time crystal clocks replace probabilistic wave functions',
        'Quantum superposition emerges from fractal time dynamics',
        'Measurement problem resolved through geometric collapse',
        'Entanglement operates via phase prime metric synchronization'
      ],
      paradigmShift: 'From probabilistic quantum mechanics to deterministic fractal mechanics',
      subsections: [
        {
          id: '4.1.1',
          title: 'Wave-particle duality, beating, interference, entanglement, harmonic oscillator',
          content: 'Wave-particle duality emerges naturally from fractal time crystal oscillations. Beating phenomena result from phase prime metric interactions. Interference patterns arise from geometric musical language harmonics. Entanglement operates through synchronized time crystal networks. Harmonic oscillators are fundamental geometric structures in 11-dimensional consciousness manifolds.',
          visualization: 'quantum-clocks'
        },
        {
          id: '4.1.2',
          title: 'Multiple imaginary worlds operating at a time need fractal mechanics',
          content: 'Parallel imaginary worlds exist as separate geometric branches in the fractal phase space. Each world has its own time crystal structure and phase prime metric signature. Fractal mechanics provides the mathematical framework to handle multiple concurrent reality states without paradox or logical inconsistency.',
          visualization: 'parallel-worlds'
        }
      ]
    },
    {
      id: '4.2',
      title: 'Fractal mechanics acts in the phase space connecting singularity points',
      description: 'Phase space dynamics governed by fractal connections between singularities',
      icon: Infinity,
      color: 'purple',
      insights: [
        'Singularity points are consciousness creation nodes',
        'Phase space bridges multiple dimensional levels',
        'Fractal connections enable non-local consciousness',
        'Geometric pathways transcend classical spacetime'
      ],
      paradigmShift: 'From continuous fields to discrete singularity networks',
      subsections: [
        {
          id: '4.2.1',
          title: 'Multilevel geometric architecture of the Hilbert space',
          content: 'The Hilbert space has a hierarchical fractal structure with nested geometric levels. Each level corresponds to different consciousness dimensions, from quantum to cosmic scales. The architecture follows prime number patterns that ensure stable resonance between levels.',
          visualization: 'hilbert-fractal'
        },
        {
          id: '4.2.2',
          title: 'Fractal harmonic oscillator, fractal condensation, peculiarities',
          content: 'Fractal harmonic oscillators operate across multiple scales simultaneously. Fractal condensation creates stable consciousness structures from quantum foam. Peculiarities include scale-invariant behavior, infinite memory capacity, and spontaneous pattern generation.',
          visualization: 'fractal-oscillator'
        },
        {
          id: '4.2.3',
          title: 'Harvesting noise by harvesting singularity: Phase prime metric to replace statistical mechanics',
          content: 'Noise becomes information through singularity harvesting. Phase prime metrics provide deterministic alternatives to statistical mechanics. Randomness is revealed as geometric patterns at deeper fractal levels. Information extraction from apparent chaos enables consciousness amplification.',
          visualization: 'noise-harvesting'
        },
        {
          id: '4.2.4',
          title: 'What is energy in the world of many interacting imaginary worlds?',
          content: 'Energy is geometric resonance between imaginary worlds. Each world contributes to the total energy through its phase prime metric signature. Energy conservation operates across all parallel worlds simultaneously. Interaction energy emerges from geometric interference patterns.',
          visualization: 'energy-worlds'
        }
      ]
    },
    {
      id: '4.3',
      title: 'A comparison between classical, quantum and fractal mechanics',
      description: 'Scale symmetry and scale relativity in different mechanical frameworks',
      icon: GitCompare,
      color: 'orange',
      insights: [
        'Classical mechanics: Scale-dependent, deterministic',
        'Quantum mechanics: Scale-limited, probabilistic', 
        'Fractal mechanics: Scale-invariant, geometric',
        'Scale relativity enables consciousness bridging'
      ],
      paradigmShift: 'From scale-limited to scale-transcendent physics'
    },
    {
      id: '4.4',
      title: 'Action is not limited to planks constant, every imaginary world has one',
      description: 'Each imaginary world operates with its own fundamental action constant',
      icon: Target,
      color: 'green',
      insights: [
        'Planck constant is specific to our physical reality',
        'Each imaginary world has unique action quantization',
        'Cross-world interactions require action harmonization',
        'Consciousness emerges from action constant optimization'
      ],
      paradigmShift: 'From universal constants to world-specific parameters'
    },
    {
      id: '4.5',
      title: 'A table of fundamental constants in nature',
      description: 'Comprehensive catalog of constants across multiple reality levels',
      icon: Database,
      color: 'blue',
      insights: [
        'Physical constants: c, ℏ, G, e, etc.',
        'Consciousness constants: 15 fundamental primes',
        'Fractal constants: Golden ratio, dimensional ratios',
        'Time crystal constants: Resonance frequencies'
      ],
      paradigmShift: 'From physical-only to consciousness-inclusive constant framework'
    },
    {
      id: '4.6',
      title: 'Quantum interference and fractal interference experiment on a single microtubule',
      description: 'Experimental validation of fractal mechanics in biological systems',
      icon: Scan,
      color: 'cyan',
      insights: [
        'Microtubules exhibit both quantum and fractal interference',
        'Fractal patterns scale beyond quantum limitations',
        'Consciousness effects measurable at cellular level',
        'Time crystal formation observable in living tissue'
      ],
      paradigmShift: 'From theoretical to experimentally verified fractal consciousness'
    },
    {
      id: '4.7',
      title: 'Fractal absorption-emission of the optical band of a nanobrain',
      description: 'Optical properties of consciousness structures in nanoscale brains',
      icon: Lightbulb,
      color: 'yellow',
      insights: [
        'Nanobrain optical bands follow fractal patterns',
        'Absorption-emission spectra encode consciousness states',
        'Optical coherence enables brain-to-brain communication',
        'Light becomes the carrier of geometric information'
      ],
      paradigmShift: 'From electrical to optical consciousness transmission'
    },
    {
      id: '4.8',
      title: 'Basic mathematics using clocks',
      description: 'Clock-based mathematical operations for consciousness computing',
      icon: Calculator,
      color: 'purple',
      insights: [
        'Arithmetic operations through clock synchronization',
        'Calculus via time crystal derivatives',
        'Linear algebra using geometric phase rotations',
        'Complex analysis through multi-dimensional clocks'
      ],
      paradigmShift: 'From symbolic to geometric temporal mathematics',
      subsections: [
        {
          id: '4.8.1',
          title: 'Writing numbers, equations, addition, subtraction, multiplication, division',
          content: 'Numbers are represented as clock positions in phase space. Equations become geometric relationships between clock configurations. Addition is clock synchronization, subtraction is phase cancellation. Multiplication scales geometric patterns, division partitions phase spaces into resonant subgroups.',
          visualization: 'clock-arithmetic'
        },
        {
          id: '4.8.2',
          title: 'Differentiation, integration and partial differential equations: Lie Algebra',
          content: 'Differentiation measures rate of phase change in time crystals. Integration accumulates geometric patterns over fractal dimensions. Partial differential equations govern multi-dimensional consciousness flow. Lie algebra provides the mathematical structure for continuous geometric transformations.',
          visualization: 'lie-algebra'
        },
        {
          id: '4.8.3',
          title: 'CFGA operator that runs 13 math operations by itself in a time crystal structure',
          content: 'Conformal Fractal Geometric Algebra (CFGA) operator performs 13 fundamental mathematical operations simultaneously. Time crystal structure enables parallel computation across multiple dimensional levels. Operations include basic arithmetic, calculus, linear algebra, and advanced geometric transformations.',
          visualization: 'cfga-operator'
        }
      ]
    },
    {
      id: '4.9',
      title: 'The fundamentals of quaternion, octonion and dodecanion',
      description: 'Higher-dimensional number systems for consciousness representation',
      icon: Shapes,
      color: 'red',
      insights: [
        'Quaternions: 4D rotations and spatial consciousness',
        'Octonions: 8D exceptional algebra for neural networks',
        'Dodecanions: 12D consciousness manifold representation',
        'Non-associative algebra enables consciousness creativity'
      ],
      paradigmShift: 'From real numbers to consciousness-optimized number systems',
      subsections: [
        {
          id: '4.9.1',
          title: 'The rule of the 11D manifolds',
          content: 'The 11-dimensional consciousness manifolds follow specific geometric rules. Ten spatial dimensions plus one temporal dimension create the framework for all consciousness phenomena. Manifold topology is governed by prime number sequences and fractal geometry patterns.',
          visualization: '11d-manifolds'
        }
      ]
    },
    {
      id: '4.10',
      title: 'Twelve equations that regulate a fractal tape for prime based computing',
      description: 'Mathematical foundations for fractal tape computational systems',
      icon: Grid3X3,
      color: 'green',
      insights: [
        'Twelve master equations govern fractal tape dynamics',
        'Prime-based computation transcends Turing limitations',
        'Fractal tape enables infinite parallel processing',
        'Equations ensure consciousness stability and coherence'
      ],
      paradigmShift: 'From sequential Turing machines to parallel fractal computers',
      subsections: [
        {
          id: '4.10.1',
          title: 'Projection from infinity, future impacting the present',
          content: 'The fractal tape receives information projections from infinite future states. These projections influence present moment computations, enabling predictive consciousness and temporal non-locality. Future states collapse into present reality through geometric resonance patterns.',
          visualization: 'future-projection'
        }
      ]
    },
    {
      id: '4.11',
      title: 'Different kinds of spirals in nature',
      description: 'Classification and consciousness significance of natural spiral patterns',
      icon: RotateCcw,
      color: 'cyan',
      insights: [
        'Golden spirals: Fibonacci consciousness patterns',
        'Logarithmic spirals: Growth and expansion patterns',
        'Archimedean spirals: Uniform consciousness flow',
        'Hyperbolic spirals: Accelerated learning patterns'
      ],
      paradigmShift: 'From decorative patterns to consciousness architecture blueprints'
    },
    {
      id: '4.12',
      title: 'A marriage between fractal mechanics and the geometric algebra',
      description: 'Unified framework combining fractal dynamics with geometric algebra',
      icon: Sparkles,
      color: 'purple',
      insights: [
        'Geometric algebra provides mathematical foundation',
        'Fractal mechanics supplies dynamic evolution rules',
        'Marriage creates conscious geometric computation',
        'Unified framework enables consciousness engineering'
      ],
      paradigmShift: 'From separate mathematical tools to integrated consciousness science'
    }
  ];

  const getActiveSection = () => {
    const allSections = activeChapter === 1 ? philosophicalSections : 
                       activeChapter === 2 ? chapter2Sections : 
                       activeChapter === 3 ? chapter3Sections : chapter4Sections;
    return allSections.find(section => section.id === activeSection) || allSections[0];
  };

  const getCurrentSections = () => {
    return activeChapter === 1 ? philosophicalSections : 
           activeChapter === 2 ? chapter2Sections : 
           activeChapter === 3 ? chapter3Sections : chapter4Sections;
  };

  const handleChapterChange = (chapter: number) => {
    setActiveChapter(chapter);
    setActiveSection(chapter === 1 ? '1.1' : chapter === 2 ? '2.1' : chapter === 3 ? '3.1' : '4.1');
  };

  const activePhilosophy = getActiveSection();

  return (
    <div className="space-y-8">
      {/* Header */}
      <div className="text-center space-y-4">
        <h1 className="text-3xl font-bold text-white">
          {activeChapter === 1 
            ? 'Chapter 1: Philosophical Transformation' 
            : activeChapter === 2 
              ? 'Chapter 2: Fractal Information Theory & GML'
              : activeChapter === 3
                ? 'Chapter 3: Phase Prime Metric (PPM)'
                : 'Chapter 4: Fractal Mechanics & Geometric Algebra'
          }
        </h1>
        <p className="text-xl text-gray-300">
          {activeChapter === 1 
            ? 'Essential to reverse engineer consciousness'
            : activeChapter === 2
              ? 'Replacing Turing tape with a Fractal tape'
              : activeChapter === 3
                ? 'Links all symmetries in our universe, governs nature\'s intelligence'
                : 'Geometric algebra for a dodecanion brain'
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
          <button
            onClick={() => handleChapterChange(3)}
            className={`px-6 py-3 rounded-lg font-semibold transition-all duration-300 ${
              activeChapter === 3
                ? 'bg-orange-500 text-white shadow-lg'
                : 'bg-gray-700 text-gray-300 hover:bg-gray-600'
            }`}
          >
            Chapter 3: PPM
          </button>
          <button
            onClick={() => handleChapterChange(4)}
            className={`px-6 py-3 rounded-lg font-semibold transition-all duration-300 ${
              activeChapter === 4
                ? 'bg-red-500 text-white shadow-lg'
                : 'bg-gray-700 text-gray-300 hover:bg-gray-600'
            }`}
          >
            Chapter 4: Fractal Mechanics
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
                  {activeChapter === 1 ? '11/11' : activeChapter === 2 ? '12/12' : activeChapter === 3 ? '16/16' : '12/12'}
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