import React, { useState, useEffect } from 'react';
import { FractalPattern } from '../types';
import { Layers, TrendingUp, Zap, RotateCw } from 'lucide-react';

export const FractalInformationPanel: React.FC = () => {
  const [patterns, setPatterns] = useState<FractalPattern[]>([]);
  const [selectedPattern, setSelectedPattern] = useState<FractalPattern | null>(null);

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

    generatePatterns();
    const interval = setInterval(generatePatterns, 2000);
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
      <div className="flex items-center justify-between">
        <h2 className="text-xl font-bold text-white flex items-center space-x-2">
          <Layers className="text-cyan-400" size={24} />
          <span>Fractal Information Theory</span>
        </h2>
        <div className="text-sm text-gray-400">
          GML Processing Active
        </div>
      </div>

      <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
        {/* Pattern List */}
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

        {/* Pattern Analysis */}
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
    </div>
  );
};