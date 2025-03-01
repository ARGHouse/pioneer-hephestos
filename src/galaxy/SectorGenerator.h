// Copyright © 2008-2025 Pioneer Developers. See AUTHORS.txt for details
// Licensed under the terms of the GPL v3. See licenses/GPL-3.txt

#ifndef SECTORGENERATOR_H
#define SECTORGENERATOR_H

#include "GalaxyConfig.h"
#include "GalaxyGenerator.h"
#include "Random.h"
#include "Random64.h"
#include "RefCounted.h"
#include "Sector.h"
#include "StarSystem.h"

class SectorCustomSystemsGenerator : public SectorGeneratorStage {
public:
	SectorCustomSystemsGenerator(int customOnlyRadius) :
		m_customOnlyRadius(customOnlyRadius) {
		m_galaxyConfig = GalaxyConfig();

		m_GalaxyExploredMax = m_galaxyConfig.Int("GalaxyExploredMax");
		m_GalaxyExploredMin = m_galaxyConfig.Int("GalaxyExploredMin");
		m_GalaxyExploredMix = m_galaxyConfig.Int("GalaxyExploredMix");
	}
	virtual bool Apply(Random &rng, Random64& rng64, RefCountedPtr<Galaxy> galaxy, RefCountedPtr<Sector> sector, GalaxyGenerator::SectorConfig *config);

private:
	int m_GalaxyExploredMax;
	int m_GalaxyExploredMin;
	int m_GalaxyExploredMix;

	int m_customOnlyRadius;
	GalaxyConfig m_galaxyConfig;
};

class SectorRandomSystemsGenerator : public SectorGeneratorStage {
public:
	SectorRandomSystemsGenerator(){
		m_galaxyConfig = GalaxyConfig();

		m_GalaxyExploredMax = m_galaxyConfig.Int("GalaxyExploredMax");
		m_GalaxyExploredMin = m_galaxyConfig.Int("GalaxyExploredMin");
		m_GalaxyExploredMix = m_galaxyConfig.Int("GalaxyExploredMix");
	}
	virtual bool Apply(Random &rng, Random64 &rng64, RefCountedPtr<Galaxy> galaxy, RefCountedPtr<Sector> sector, GalaxyGenerator::SectorConfig *config);

private:
	int m_GalaxyExploredMax;
	int m_GalaxyExploredMin;
	int m_GalaxyExploredMix;

	const std::string GenName(RefCountedPtr<Galaxy> galaxy, const Sector &sec, Sector::System &sys, int si, Random &rand, Random64& rand64 );
	GalaxyConfig m_galaxyConfig;
};

class SectorPersistenceGenerator : public SectorGeneratorStage {
public:
	SectorPersistenceGenerator(GalaxyGenerator::Version version) :
		m_version(version) {}
	virtual bool Apply(Random &rng, Random64& rng64, RefCountedPtr<Galaxy> galaxy, RefCountedPtr<Sector> sector, GalaxyGenerator::SectorConfig *config);
	virtual void FromJson(const Json &jsonObj, RefCountedPtr<Galaxy> galaxy);
	virtual void ToJson(Json &jsonObj, RefCountedPtr<Galaxy> galaxy);

private:
	void SetExplored(Sector::System *sys, StarSystem::ExplorationState e, double time);

	const GalaxyGenerator::Version m_version;

	// m_exploredSystems stores the date (in-game) when each system was explored.
	// = 0  ->  System was already explored at the start of the game.
	// > 0  ->  System explored at the specified date.
	// < 0  ->  System is unexplored.
	// Systems that don't appear in the map are also unexplored.
	//
	// The date value is day/month/year, bit-packed into a 32-bit integer.
	// Low 5 bits (bits 0..4) -> Day.
	// Middle 4 bits (bits 5..8) -> Month.
	// High bits (bits 9..) -> Year.
	std::map<SystemPath, Sint32> m_exploredSystems;
};

#endif
