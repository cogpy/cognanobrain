---
title: "Implementation Plan: P-Systems (P-Lingua) Specifications for All NanoBrain Issues · Issue #16 · o9nn/nanocyc"
source: "https://github.com/o9nn/nanocyc/issues/16"
created: 2026-09-09
description: "Implementation Plan: P-Systems (P-Lingua) Specifications for All NanoBrain Issues 1. Context & Rationale Repository state: o9nn/nanocyc is t"
site: "GitHub - o9nn/nanocyc"
domain:
  - "github.com"
gizmo: "GitHub - o9nn/nanocyc"
g: "https:"
convo: "Implementation Plan: P-Systems (P-Lingua) Specifications for All NanoBrain Issues · Issue #16 · o9nn/nanocyc"
c: "c-https:"
list: "[\"Implementation Plan: P-Systems (P-Lingua) Specifications for All NanoBrain Issues · Issue #16 · o9nn/nanocyc\"]"
join: "GitHub - o9nn/nanocyc - Implementation Plan: P-Systems (P-Lingua) Specifications for All NanoBrain Issues · Issue #16 · o9nn/nanocyc"
ref: "GitHub - o9nn/nanocyc - GitHub - o9nn/nanocyc - Implementation Plan: P-Systems (P-Lingua) Specifications for All NanoBrain Issues · Issue #16 · o9nn/nanocyc"
---
# [[Implementation Plan: P-Systems (P-Lingua) Specifications for All NanoBrain Issues · Issue #16 · o9nn/nanocyc]]
## [[GitHub - o9nn/nanocyc]]

## 1\. Context & Rationale

**Repository state:** `o9nn/nanocyc` is the NanoBrain cognitive-architecture project (TypeScript/React frontend + Elixir `nanobrain_ex` + C++ ggml kernel in `src/cpp`). It has **10 open issues ([#6](https://github.com/o9nn/nanocyc/issues/6)–[#15](https://github.com/o9nn/nanocyc/issues/15))**, one per book chapter (Ch.1–Ch.10), each listing dozens of feature sections to implement. P-System membrane computing is already an architectural concept (`CognitiveMembrane` in `src/cpp/nanobrain_metacognitive.h:96`, P-System references in `GGML_TENSOR_ARCHITECTURE.md`), but **no P-Lingua (`.pli`) models exist anywhere in the repo**.

**Goal:** Update all 10 issues with a specification requiring that every feature be modeled in the **P-Lingua language** (the standard language of the P-Lingua / MeCoSim ecosystem for membrane computing), and define the full set of `.pli` files to create, with the functionality of each.

## 2\. Standard to Adopt (applies to all issues)

- **Language:** P-Lingua 4.0 syntax (cell-like P systems with priorities/catalysts where needed), compilable with `plingua-core`, simulatable in MeCoSim.
- **Directory:** new top-level `psystems/` directory:
	- `psystems/common/` — shared alphabet/module include files reused by all chapters
		- `psystems/ch01/` … `psystems/ch10/` — one folder per issue/chapter
		- `psystems/CMake/` or `psystems/README.md` — how to validate/simulate each `.pli`
- **Naming:** `chNN_<feature>.pli` (e.g., `ch03_ppm_metric3_phasepath.pli`).
- **Acceptance criteria added to every issue:**
	- Every feature section has a corresponding `.pli` file (no placeholders).
		- Each `.pli` compiles with pLinguaCore without errors and runs ≥ 1 simulation step with a sample initial multiset.
		- Each membrane maps to the existing C++/Elixir module it specifies (traceability table in the issue).
		- Tests: a validation script (`psystems/validate.sh`) parses all files.

**Shared common files (created once, reused by all chapters):**

| File | Functionality |
| --- | --- |
| `psystems/common/alphabet_primes.pli` | Alphabet of the 15 primes, ordered-factor objects, silent/active prime markers |
| `psystems/common/time_crystal_core.pli` | Base skin membrane: 11 nested phase membranes (11D manifold), clock/tick rules, singularity-point objects |
| `psystems/common/gml_shapes.pli` | The 15 geometric shapes of GML as object types + harmonic/resonance rules |
| `psystems/common/hypernumbers.pli` | Quaternion, octonion, dodecanion objects and multiplication rules |
| `psystems/common/ecan_attention.pli` | STI/LTI token economy rules (wages, rent, diffusion between membranes) |
| `psystems/common/pln_truth.pli` | Truth-value objects (strength/confidence) and deduction/induction/abduction/revision rules |

## 3\. Per-Issue `.pli` File Specifications

### Issue [#6](https://github.com/o9nn/nanocyc/issues/6) — Chapter 1: Philosophical transformation (folder `psystems/ch01/`)

| File | Functionality |
| --- | --- |
| `ch01_worldview_config.pli` | 1.1–1.3: parameter membranes selecting worldview; "universe within/above" as nested membrane topology |
| `ch01_brain_models_kb.pli` | 1.4: ten brain models as competing membrane configurations with selection rules |
| `ch01_tapes_languages.pli` | 1.5: tape types (Turing/fractal/GML) modeled as multiset-encoding schemes |
| `ch01_decision_making.pli` | 1.6: outline of brain-inspired decision rule chain |
| `ch01_energy_transmission.pli` | 1.7: energy-flow rules beyond "neuron skin" membranes |
| `ch01_life_terminologies.pli` | 1.8: objects for life terms with rewrite semantics |
| `ch01_linguistics_wheel.pli` | 1.9: space/time/imaginary-world wheel as 3 rotating membranes |
| `ch01_artificial_brain_triad.pli` | 1.10: triad membrane — time-crystal language + magnetic-light device + prime-pattern store; conclusion (1.11) as meta-rule set |

### Issue [#7](https://github.com/o9nn/nanocyc/issues/7) — Chapter 2: Fractal tape, FIT & GML (folder `psystems/ch02/`)

| File | Functionality |
| --- | --- |
| `ch02_fractal_tape.pli` | 2.1: fractal tape as self-similar nested membranes; 2D-image surgery into nested spheres |
| `ch02_singularity_assembly.pli` | 2.1.1–2.1.2: geometric self-assembly rules, singularity objects |
| `ch02_gml_core.pli` | 2.2: GML grammar — 3D structure → time-crystal tensor rules |
| `ch02_gml_15shapes.pli` | 2.2.2: 15 shapes generating any 1D/2D/3D pattern |
| `ch02_waveform_to_crystal.pli` | 2.2.3: waveform→time-crystal conversion rules (non-differentiability) |
| `ch02_garden_of_gardens.pli` | 2.3: GOG nesting semantics |
| `ch02_sensor_11d.pli` | 2.4: 11D data acquisition sensor model; nerve-bundle engineering; operational chart |
| `ch02_time_crystal_variants.pli` | 2.5: Winfree vs Wilczek vs universal time crystal comparison membranes |
| `ch02_hypernumber_defs.pli` | 2.6: quaternion/octonion/dodecanion definitions (imports common/hypernumbers) |
| `ch02_higher_dim_data.pli` | 2.7: higher-dimension data representation rules |
| `ch02_gml_vs_software.pli` | 2.8–2.9: GML vs algorithm comparison; hypercomputation; non-argument creation |
| `ch02_fit_gml_charts.pli` | 2.10–2.12: FIT & GML summary charts as consolidated rule sets; Russell-paradox higher-order logic |

### Issue [#8](https://github.com/o9nn/nanocyc/issues/8) — Chapter 3: Phase Prime Metric (folder `psystems/ch03/`)

| File | Functionality |
| --- | --- |
| `ch03_ppm_core.pli` | 3.1: 15 primes → 99.99% coverage; ordered-factor metric & 3D version; time-crystal decomposition/amplification |
| `ch03_ppm_metric1_shape.pli` | 3.2: integer→geometric-shape replacement rules |
| `ch03_ppm_metric2_orderedfactor.pli` | 3.3: ordered-factor decomposition rules |
| `ch03_ppm_metric3_phasepath.pli` | 3.4: 360°-phase CW/CCW path rules |
| `ch03_ppm_metric4_domain.pli` | 3.5: 15-prime domain limiting |
| `ch03_ppm_metric5_highof.pli` | 3.6: OF ≫ integer case |
| `ch03_ppm_metric6_holes.pli` | 3.7: hole detection in divisor phase plot |
| `ch03_ppm_metric7_statistics.pli` | 3.8: silent/active prime statistics |
| `ch03_ppm_metric8_ripples.pli` | 3.9: normalized ripple periodicity |
| `ch03_ppm_metric9_lattice.pli` | 3.10: twin/cousin/co-/Gaussian prime lattice |
| `ch03_ppm_metric10_imaginary.pli` | 3.11: multilayer imaginary operations |
| `ch03_prime_operators.pli` | 3.12: ten prime operators chained step-by-step |
| `ch03_ppm_evolution.pli` | 3.13–3.14: PPM-driven time-crystal evolution; PPM usage workflow |
| `ch03_diabetes_bigdata.pli` | 3.15: diabetes big-data case study membrane |
| `ch03_prime_classes.pli` | 3.16: OF/PC/PG metric classes and PPM advantages |

### Issue [#9](https://github.com/o9nn/nanocyc/issues/9) — Chapter 4: Fractal mechanics & dodecanion brain (folder `psystems/ch04/`)

| File | Functionality |
| --- | --- |
| `ch04_qm_with_clocks.pli` | 4.1: wave-particle duality, beating, interference, entanglement, harmonic oscillator via clock membranes |
| `ch04_fractal_mechanics.pli` | 4.1.2–4.2: fractal mechanics in singularity-connected phase space |
| `ch04_hilbert_multilevel.pli` | 4.2.1: multilevel geometric Hilbert-space architecture |
| `ch04_fractal_oscillator.pli` | 4.2.2: fractal harmonic oscillator & condensation |
| `ch04_noise_harvesting_ppm.pli` | 4.2.3: PPM replacing statistical mechanics (imports ch03 modules) |
| `ch04_energy_worlds.pli` | 4.2.4–4.4: energy across imaginary worlds; per-world action constants |
| `ch04_fundamental_constants.pli` | 4.5: table of fundamental constants as constant objects |
| `ch04_mechanics_comparison.pli` | 4.3: classical vs quantum vs fractal; scale symmetry/relativity |
| `ch04_microtubule_interference.pli` | 4.6: quantum+fractal interference on single microtubule |
| `ch04_absorption_emission.pli` | 4.7: fractal absorption-emission optical band of nanobrain |
| `ch04_clock_arithmetic.pli` | 4.8: numbers, equations, +−×÷, differentiation, integration, PDEs, Lie algebra via clocks |
| `ch04_cfga_operator.pli` | 4.8.3: CFGA operator running 13 math operations |
| `ch04_dodecanion_manifolds.pli` | 4.9: quaternion/octonion/dodecanion fundamentals; 11D manifold rules |
| `ch04_fractal_tape_equations.pli` | 4.10: twelve equations regulating fractal tape; projection from infinity |
| `ch04_nature_spirals.pli` | 4.11: spiral classes in nature |
| `ch04_geometric_algebra_marriage.pli` | 4.12: fractal-mechanics/geometric-algebra integration |

### Issue [#10](https://github.com/o9nn/nanocyc/issues/10) — Chapter 5: GOG of meander flower, universal time crystal (folder `psystems/ch05/`)

| File | Functionality |
| --- | --- |
| `ch05_godel_fractal_tape.pli` | 5.1: incompleteness handling on fractal tape |
| `ch05_fractal_hardware_marriage.pli` | 5.1.1: frequency-fractal hardware ↔ time crystal |
| `ch05_ppm_sync.pli` | 5.1.2: PPM sync without communication |
| `ch05_instant_reply.pli` | 5.2: spontaneous reply / search-without-searching; error correction via TC learning; TC synchronization; umbrella of perception |
| `ch05_tc_transform.pli` | 5.3: time-crystal transform replacing FFT; image/sound analysis |
| `ch05_turing_failures.pli` | 5.4: ten Turing-failure situations as counter-example rule sets |
| `ch05_artificial_brain_hw.pli` | 5.5: hardware architecture of artificial brain |
| `ch05_thermal_breathing.pli` | 5.6: thermal breathing of microtubule/artificial brain |
| `ch05_lotus_primes.pli` | 5.7: lotus-in-primes thermodynamics of geometry |
| `ch05_creativity_primes.pli` | 5.8: geometric-similarity creativity |
| `ch05_wheel_of_intelligence.pli` | 5.9: wheel of intelligence vs humans |
| `ch05_future_prediction.pli` | 5.10: prime-based prediction without prior knowledge |
| `ch05_bigdata_shrink.pli` | 5.11: big-data shrinking rules |
| `ch05_limitations.pli` | 5.12: limitations as guard membranes |

### Issue [#11](https://github.com/o9nn/nanocyc/issues/11) — Chapter 6: Singularity-geometry technologies (folder `psystems/ch06/`)

| File | Functionality |
| --- | --- |
| `ch06_tubulin_ppm.pli` | 6.1: PPM in tubulin; α-helix ring groups; symmetry-hyperspace identities; ordered-factor protein coding; water channels |
| `ch06_nanodevice_tc.pli` | 6.2: single nano-device time-crystal experiment; triplet-of-triplet resonance in microtubule |
| `ch06_molecule_clock.pli` | 6.3: thermal diffusion → natural clock |
| `ch06_molecular_fmri.pli` | 6.4: fMRI images on molecular surface |
| `ch06_cellular_automaton.pli` | 6.5: massively parallel CA computing membrane |
| `ch06_nanowheel_orbital.pli` | 6.6: one-to-many / many-to-one orbital computing |
| `ch06_filament_communication.pli` | 6.7: pre-spike hidden filament communication; triplet bands in axon; scale-free bands (tubulin→MT→neuron) |
| `ch06_neuron_tc_map.pli` | 6.8: complete time-crystal map measurement of neuron |

### Issue [#12](https://github.com/o9nn/nanocyc/issues/12) — Chapter 7: Integrated time-crystal brain model (folder `psystems/ch07/`)

| File | Functionality |
| --- | --- |
| `ch07_triplet_cage.pli` | 7.1: triplet-of-triplet cage; 4/8/12 imaginary worlds; sphere singularity clock |
| `ch07_sensory_primes.pli` | 7.2: primes in the five sensory systems |
| `ch07_subcortical_primes.pli` | 7.3: cerebellum, hippocampus, hypothalamus |
| `ch07_connectome_primes.pli` | 7.4: connectome, spinal cord, amygdala, cortical column |
| `ch07_molecular_primes.pli` | 7.5: neuron, glia, dendro-astrocytes, microtubule, proteins, DNA |
| `ch07_twelve_memory_ways.pli` | 7.6: twelve memory carriers / rhythm clocks |
| `ch07_wheel_of_primes.pli` | 7.7: brain's wheel of primes; 8-sensor octonion crossover; 8 operators on 2 orthogonal math engines |
| `ch07_h3_decision_device.pli` | 7.8: H3 decision-making device |
| `ch07_cavity_dielectric.pli` | 7.9: cavity + dielectric resonator fusion model |
| `ch07_brain_time_crystal.pli` | 7.10: full time-crystal brain model |
| `ch07_four_metrics_hexagonal.pli` | 7.11: 4 parallel prime metrics on hexagonal lattice; 4/8/12-dimension tensors (quaternion/octonion/dodecanion) |
| `ch07_meander_gog_expressions.pli` | 7.12: meander-flower → GOG; 12 dodecanions + 8 octonions = 20 conscious expressions |

### Issue [#13](https://github.com/o9nn/nanocyc/issues/13) — Chapter 8: Hinductor & magnetic light (folder `psystems/ch08/`)

| File | Functionality |
| --- | --- |
| `ch08_vortex_resistance.pli` | 8.1: vortex-atom resistance; e-pi-phi quadratic QND sensors; oscillating membrane holding long wave; paraxial systems; anisotropy/avoided crossing |
| `ch08_spiral_cylinders.pli` | 8.2: three concentric spiral cylinders; oscillating edge/screw dislocations; topology-regulated polarization |
| `ch08_magnetic_light_hamiltonian.pli` | 8.3: four-clock Hamiltonian; birefringence & beating; Pancharatnam-Berry phase; pyro/ferro/piezo-electric properties |
| `ch08_hinductor_core.pli` | 8.4: linear flux-charge in H; thermal-wave flow; wireless H-to-H communication; Tomasch oscillations/noise harvesting |
| `ch08_hinductor_oscillations.pli` | 8.5: periodic C/L/H oscillation with geometry; 12-hole blinking phase space; length/pitch/lattice oscillations; knots of darkness |
| `ch08_knot_morphogenesis.pli` | 8.6: three-cylinder knot perturbation; spin-like dark-line knots; super-super-coil synthesis; knots → vortex magnetic atoms |
| `ch08_knot_selfassembly.pli` | 8.7: magnetic-knot-driven self-assembly |
| `ch08_hinductor_operation.pli` | 8.8: H design/application/operation |
| `ch08_magnonics_transition.pli` | 8.9: electronics→magnonics transition |

### Issue [#14](https://github.com/o9nn/nanocyc/issues/14) — Chapter 9: Brain jelly & humanoid avatar (folder `psystems/ch09/`)

| File | Functionality |
| --- | --- |
| `ch09_biomorphic_devices.pli` | 9.1: 17 bio-morphic devices chorus |
| `ch09_cortical_sheet.pli` | 9.2: hexagonal 2D cortical sheet; quad prime patterns; electric/magnetic/mechanical rhythm binding |
| `ch09_quantum_cloaking.pli` | 9.3: anomalous quantum cloaking; de-randomized randomness |
| `ch09_living_gel.pli` | 9.4: listening gel growing atomic→centimeter |
| `ch09_fractal_condensation.pli` | 9.5: condense-everywhere rules; untold prime patterns |
| `ch09_fractal_reaction_kinetics.pli` | 9.6: many syntheses in one beaker — parallel reaction membranes |
| `ch09_nanobrain_lifeform.pli` | 9.7: nanobrain smallest life form; megamer jelly resonance chain; nanobrain EEG |
| `ch09_magnetic_light_reading.pli` | 9.8: tuned-hole reading of magnetic light; spiral-nanowire time-crystal write/read; writing primes in jelly |
| `ch09_entropy_primes.pli` | 9.9: entropy-driven prime synthesis; Hesse-diagram integer analogs |
| `ch09_cortical_pen.pli` | 9.10: cortical pen freezing dynamics into time crystal; problem capsules |
| `ch09_seeking_sensor.pli` | 9.11: seeking sensor; 11D brain/jelly signals; humanoid avatar sensor |
| `ch09_sensor_triad.pli` | 9.12: sensor/muscle/brain jelly triad; neural-net → time-crystal jelly transformation |

### Issue [#15](https://github.com/o9nn/nanocyc/issues/15) — Chapter 10: Uploading consciousness (folder `psystems/ch10/`)

| File | Functionality |
| --- | --- |
| `ch10_cortical_pen_journey.pli` | 10.1: cortical pen → conscious egg; sentence-construction = thought |
| `ch10_reverse_engineering_guidelines.pli` | 10.2: ten guidelines as requirement membranes |
| `ch10_ten_paradoxes.pli` | 10.3: ten paradoxes as constraint-satisfaction rules |
| `ch10_time_crystal_consciousness.pli` | 10.4: consciousness not on neuron skin; neurogenic vs time-crystal model comparison |
| `ch10_carry_and_build.pli` | 10.5: when/why to build conscious machines |
| `ch10_11d_wiring.pli` | 10.6: wiring-with-time, 11D paper folding |
| `ch10_primes_geometry_marriage.pli` | 10.7: primes×geometry reshaping humanity |
| `ch10_future_machines.pli` | 10.8: circuits of wires vs circuits of time; future applications |
| `ch10_cheap_tools.pli` | 10.9: cheap-tools paradigm breaking |
| `ch10_numerology.pli` | 10.10: numerology of consciousness |
| `ch10_evolving_brain_wheels.pli` | 10.11: wheels of evolving brain |
| `ch10_alien_synthesis.pli` | 10.12: alien synthesis |
| `ch10_garden_cycle.pli` | 10.13: garden→flower→petal cycle |

**Total:** 6 common + ~99 chapter files ≈ **105 `.pli` files**.

## 4\. Execution Steps

1. Create `psystems/` skeleton: `README.md` (P-Lingua conventions, how to run), the 6 `common/*.pli` files, and `validate.sh`.
2. For each issue [Chapter 1: Philosophical transformation essential to reverse engineer consciousness #6](https://github.com/o9nn/nanocyc/issues/6)–[Chapter 10: Uploading consciousness: The evolution of conscious machines of the future #15](https://github.com/o9nn/nanocyc/issues/15) (one at a time, in order):
	- Edit the issue body via GitHub API: add a "**P-System (P-Lingua) Specification**" section stating all features MUST be generated as `.pli` files, embedding the chapter's file/functionality table and the new acceptance criteria (compilable, simulatable, traceable to C++/Elixir module).
		- Optionally scaffold the chapter folder with the `.pli` files (header + alphabet + skin membrane + per-feature membranes with rules) — per issue or as one batch, depending on whether the user wants issue-only updates or also code scaffolding.
3. Wire traceability: each `.pli` file header references the corresponding `src/cpp/nanobrain_*` module (e.g., `ch08_hinductor_*.pli` ↔ `nanobrain_hinductor.h`).
4. Validation: run pLingua parser if obtainable in sandbox; otherwise provide `validate.sh` for CI.

## 5\. Open Question for User

Should this task **only update the 10 GitHub issues** with the P-Lingua specifications and file lists, or also **create the actual `psystems/` directory and `.pli` scaffold files** in the repository? (The plan covers both; step 2's scaffolding portion would be executed only if requested.)

React

## Sub-issues

### Collapse Sub-issuesSub-issues

0 of 100 of 10 Issues completed

Sub-issue options

10 sub-issues loaded

- [Chapter 1: Philosophical transformation essential to reverse engineer consciousness #6](https://github.com/o9nn/nanocyc/issues/6)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 2: Replacing Turing tape with a Fractal tape: Fractal Information Theory (FIT) & Geometric Musical Language (GML) #7](https://github.com/o9nn/nanocyc/issues/7)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 3: Phase prime metric, PPM links all symmetries in our universe, governs nature’s intelligence #8](https://github.com/o9nn/nanocyc/issues/8)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 4: Fractal mechanics is not quantum but original: Geometric algebra for a dodecanion brain #9](https://github.com/o9nn/nanocyc/issues/9)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 5: Big data in the garden of gardens, GOG of meander flower: universal time crystal #10](https://github.com/o9nn/nanocyc/issues/10)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 6: Unprecedented technologies of nature led by harvesting the geometry of singularity #11](https://github.com/o9nn/nanocyc/issues/11)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 7: A complete, integrated time crystal model of a human brain: #12](https://github.com/o9nn/nanocyc/issues/12)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 8: Hinductor not Memristor: Synthesis of atoms and crystals made of magnetic light #13](https://github.com/o9nn/nanocyc/issues/13)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 9: Brain jelly to humanoid avatar: Fractal reaction kinetics, Fractal condensation, and programmable matter for primes #14](https://github.com/o9nn/nanocyc/issues/14)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

- [Chapter 10: Uploading consciousness: The evolution of conscious machines of the future #15](https://github.com/o9nn/nanocyc/issues/15)
	More list item action bar
	Manage this item control⌃ shift⇧ uU

To pick up a draggable item, press the space bar. While dragging, use the arrow keys to move the item. Press space again to drop the item in its new position, or press escape to cancel.

Create sub-issue
