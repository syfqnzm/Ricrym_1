# Ricrym_1

Features & Implemented Plugins
1. Enhanced Input System
  -Why? Unreal Engine 5 replaced the old input system with Enhanced Input, which provides better handling for complex inputs, such as combos and action-based mechanics.

- Changes in Build.cs:
  - We modified Ricrym_1.Build.cs to enable the Enhanced Input plugin:
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Networking", "Sockets", "EnhancedInput" });

This allows us to use:
UEnhancedInputComponent for binding inputs.
UInputMappingContext for managing input settings dynamically.
UInputAction for defining movement, attacks, and other actions.

ustom Character (PlayerCharacter)
Implemented a custom PlayerCharacter derived from ACharacter, supporting:
  - Enhanced movement (WASD + Mouse Look).
  - Jumping mechanics.
  - Combat system placeholder (Melee/Ranged).

2. Key Functions in PlayerCharacter.cpp

::SetupPlayerInputComponent()	
  - Binds movement, look, and jump inputs using Enhanced Input.
::Move()
  - Handles forward/backward & strafe movement.
::Look()
  - Controls camera rotation with mouse input.
::StartJump()
  - Triggers jump action.
::StopJump()
  -  Stops jumping when releasing Spacebar.
::BeginPlay()
  - Ensures the player possesses the correct character at runtime.

3. Health System (HealthComponent)
Implemented a modular Health System (UHealthComponent) to manage player and enemy health.

Key Functions in HealthComponent.cpp
::TakeDamage(float DamageAmount)	
  - Reduces health and destroys the character when it reaches 0.

4. Animation System Integration
  - Linked MyPlayerCharacter to ABP_ThirdPerson for animations.
  - Ensured the correct State Machine for movement.
  - Used ThirdPerson_BS (Blend Space) for smooth running animations.

5. Multiplayer Support (Initial Setup)
  - Enabled bReplicates = true; in character classes.
  - Implemented replication for health using DOREPLIFETIME().
