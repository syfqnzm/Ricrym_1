// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "PlayerCharacter.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
#ifdef RICRYM_1_PlayerCharacter_generated_h
#error "PlayerCharacter.generated.h already included, missing '#pragma once' in PlayerCharacter.h"
#endif
#define RICRYM_1_PlayerCharacter_generated_h

#define FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execAttack); \
	DECLARE_FUNCTION(execTakeDamage);


#define FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAPlayerCharacter(); \
	friend struct Z_Construct_UClass_APlayerCharacter_Statics; \
public: \
	DECLARE_CLASS(APlayerCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/Ricrym_1"), NO_API) \
	DECLARE_SERIALIZER(APlayerCharacter)


#define FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	APlayerCharacter(APlayerCharacter&&); \
	APlayerCharacter(const APlayerCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APlayerCharacter); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APlayerCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(APlayerCharacter) \
	NO_API virtual ~APlayerCharacter();


#define FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h_9_PROLOG
#define FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h_12_INCLASS_NO_PURE_DECLS \
	FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> RICRYM_1_API UClass* StaticClass<class APlayerCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Ricrym_1_Source_Ricrym_1_PlayerCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
