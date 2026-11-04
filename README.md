# An Exploration of Steering Behaviors

A graduate school project (FIEA, 2019) of steering behaviors for autonomous characters after Craig Reynolds, in Unreal Engine and C++. Behavior trees in Blueprint decide which steering behaviors an agent runs. The code is the original UE4 project brought forward to run on Unreal 5.6. It is a study project with a limited scope, and it has not had an optimization pass, so frame times are high with many agents in the level.

## Running the demo

1. Open the project in Unreal Engine 5.6. The editor opens on `Content/Maps/FinalDemoMap`; if it does not, open that map from the Content Browser.
2. Press Play. You are a spectator pawn: WASD to move, Space to rise, mouse to look. The spectator passes through geometry (no-clip) at all times.
3. Press 1 and 2 to step through the demo behaviors. The behavior each boid is running is shown as a line of text at the top left of the viewport, one line per behavior in use. Boids with different controller lists can be on different behaviors at once, so more than one line can show.
4. C switches the camera. V enables click events; with them on, click and drag to spin the camera around the character.
5. 3, 4 and 5 toggle the engine's profiling stats on screen: 3 runs stat fps and stat unit, 4 runs stat ai and stat AiBehaviorTree, 5 runs stat game. Escape quits.

Other maps in `Content/Maps`: SeekPursueMap, CollisionAvoidancePursueMap, FinalDemoMap100 (a smaller population, which runs better), FinalDemoMapNight, and two test maps.

## What is here

The steering behaviors, in C++, one class each under `Source/Holistic_Pathfinding/Public`, all deriving from `ReynoldsSteeringBehavior`:

- Individual: Seek, Flee, Arrive, Pursue, Evade, Wander, Rest (the project's own, a stop)
- Steering with the environment: Path Follow, Interpose, Containment, Avoid Obstacles, Avoid Collisions
- Group: Separation, Alignment, Cohesion, Flocking

The runtime pieces, also C++:

- `ReynoldsPawn`: the agent. It holds its list of AI controllers and starts on the first.
- `ReynoldsAIController`: holds one instance of each steering behavior and the list of those currently active, and combines their weighted steering vectors into one, clamped to the pawn's maximum speed.
- `ReynoldsSteeringComponent`: the same steering math as Blueprint-callable functions over the pawn's movement component, used by three of the smart boids.

The decision layer, in Blueprint under `Content/Blueprints`:

- `BehaviorTree/`: the eight behavior trees (tag, predator, prey flocking, flocking-wander, contain-separate, contain-separate-wander, smart path following, arrive-pulse), their blackboard, and the tasks and decorators they use. The tasks add and clear steering behaviors on the agent's controller. The decorators ask about the agent's state: hungry, fleeing, aware of a predator, on its path, avoiding an obstacle.
- `Boids/`: the AI controllers, one per demo behavior, seven of which run those trees, and the boid pawns, including the smart boids (predator, flocking prey, tag boid, thinking path follower, moth, forager).
- `BoidDebugController`, `SplineActor` (paths for path-following agents), `DemoTextBillboard`, and the spectator game mode.

## Reference

Craig W. Reynolds, "Steering Behaviors For Autonomous Characters," Game Developers Conference, 1999.
