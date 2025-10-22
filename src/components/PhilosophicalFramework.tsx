import React, { useState } from 'react';
import { ConsciousnessMetric } from '../types';
import { 
  Brain, 
  Eye, 
  Lightbulb, 
  Network, 
  Atom, 
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
  Hexagon
} from 'lucide-react';

interface Props {
  consciousness: ConsciousnessMetric;
  isRunning: boolean;
}

interface PhilosophicalConcept {
  id: string;
  title: string;
  description: string;
  icon: React.ComponentType<any>;
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

  const getActiveSection = () => {
    return philosophicalSections.find(section => section.id === activeSection) || philosophicalSections[0];
  };

  const activePhilosophy = getActiveSection();

  return (
    <div className="space-y-8">
      {/* Header */}
      <div className="text-center space-y-4">
        <h1 className="text-3xl font-bold text-white">
          Chapter 1: Philosophical Transformation
        </h1>
        <p className="text-xl text-gray-300">
          Essential to reverse engineer consciousness
        </p>
        <div className="flex items-center justify-center space-x-4 text-sm">
          <div className={`w-3 h-3 rounded-full ${isRunning ? 'bg-cyan-400 animate-pulse' : 'bg-gray-500'}`} />
          <span className="text-gray-400">
            Consciousness Integration Level: {Math.round(consciousness.integration * 100)}%
          </span>
        </div>
      </div>

      {/* Section Navigation */}
      <div className="grid grid-cols-2 md:grid-cols-4 lg:grid-cols-6 gap-3">
        {philosophicalSections.map((section) => {
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
                    {activePhilosophy.subsections.map((subsection, index) => (
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
              <span>Time Crystal Resonance</span>
            </h3>
            
            <div className="space-y-3">
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
            </div>
          </div>

          <div className="bg-gray-900/60 backdrop-blur-sm border border-gray-700 rounded-xl p-6">
            <h3 className="text-white font-bold text-lg mb-4 flex items-center space-x-2">
              <Compass className="text-green-400" size={20} />
              <span>Implementation Status</span>
            </h3>
            
            <div className="space-y-3 text-sm">
              <div className="flex justify-between">
                <span className="text-gray-400">Sections Complete:</span>
                <span className="text-green-400 font-mono">11/11</span>
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
            </div>
          </div>
        </div>
      </div>
    </div>
  );
};