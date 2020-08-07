DROP TABLE IF EXISTS fvaDeviceTypes;
CREATE TABLE fvaDeviceTypes	(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL);
.separator ,
.import "../../fva/#data#/fvaDeviceTypes.csv" fvaDeviceTypes

DROP TABLE IF EXISTS fvaDevices;
CREATE TABLE fvaDevices		(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, 
				OwnerId INTEGER REFERENCES fvaPeople (ID) ON DELETE RESTRICT MATCH SIMPLE NOT NULL, 
				LinkedName VARCHAR NOT NULL,
				Name VARCHAR NOT NULL, 
				fvaDeviceType INTEGER NOT NULL REFERENCES fvaDeviceTypes (ID) ON DELETE RESTRICT MATCH SIMPLE);
separator ,
.import "../../fva/#data#/fvaDevices.csv" fvaDevices

DROP TABLE IF EXISTS fvaEventTypes;
CREATE TABLE fvaEventTypes		(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL);
separator ,
.import "../../fva/#data#/fvaEventTypes.csv" fvaEventTypes

DROP TABLE IF EXISTS fvaEventRelations;
CREATE TABLE fvaEventrelations	(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL, 
				Type INTEGER NOT NULL REFERENCES fvaEventTypes (ID) ON DELETE RESTRICT MATCH SIMPLE);
separator ,
.import "../../fva/#data#/fvaEventRelations.csv" fvaEventRelations

DROP TABLE IF EXISTS fvaRelationTypes;
CREATE TABLE fvaRelationTypes	(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL);
separator ,
.import "../../fva/#data#/fvaEventRelations.csv" fvaEventRelations

DROP TABLE IF EXISTS fvaPeopleRelations;
CREATE TABLE fvaPeopleRelations	(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL,
				RelationType INTEGER NOT NULL REFERENCES fvaRelationTypes (ID) ON DELETE RESTRICT MATCH SIMPLE);
separator ,
.import "../../fva/#data#/fvaPeopleRelations" fvaPeopleRelations

DROP TABLE IF EXISTS fvaPeople;
CREATE TABLE fvaPeople		(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL, 
				FullName VARCHAR NOT NULL, 
				RelationId INTEGER NOT NULL REFERENCES fvaPeopleRelations (ID) ON DELETE RESTRICT MATCH SIMPLE, 
				RelPersonID INTEGER NOT NULL REFERENCES fvaPeople (ID) ON DELETE RESTRICT MATCH SIMPLE);
separator ,
.import "../../fva/#data#/fvaPeople" fvaPeople

DROP TABLE IF EXISTS fvaPlaceTypes;
CREATE TABLE fvaPlaceTypes	(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL);
separator ,
.import "../../fva/#data#/fvaPlaceTypes" fvaPlaceTypes

DROP TABLE IF EXISTS fvaPlaces;
CREATE TABLE fvaPlaces		(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL, 
				Type INTEGER NOT NULL REFERENCES fvaPlaceTypes (ID) ON DELETE RESTRICT MATCH SIMPLE);
separator ,
.import "../../fva/#data#/fvaPlaces" fvaPlaces

DROP TABLE IF EXISTS fvaParams;
CREATE TABLE fvaParams     	(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL, 
				Value VARCHAR NOT NULL);
separator ,
.import "../../fva/#data#/fvaParams" fvaParams

DROP TABLE IF EXISTS fvaFolder;
CREATE TABLE fvaFolder		(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL,
				DevId INTEGER NOT NULL REFERENCES fvaDevices (ID) ON DELETE RESTRICT MATCH SIMPLE,
				Tags		VARCHAR,
				People		VARCHAR,
				PlaceId INTEGER REFERENCES fvaPlaces (ID) ON DELETE RESTRICT MATCH SIMPLE,
				EventId INTEGER REFERENCES fvaEventRelations (ID) ON DELETE RESTRICT MATCH SIMPLE,
				ReasonPeople	VARCHAR,   
				LinkedFolder	VARCHAR,
				whoTookFotoId   INTEGER REFERENCES fvaPeople (ID) ON DELETE RESTRICT MATCH SIMPLE, 
				ScanerId INTEGER REFERENCES fvaDevices (ID) ON DELETE RESTRICT MATCH SIMPLE
				);
.separator ,
.import "../../fva/#data#/fvaFolder.csv" fvaFolder

DROP TABLE IF EXISTS fvaFile;
CREATE TABLE fvaFile		(ID INTEGER PRIMARY KEY UNIQUE NOT NULL, Name VARCHAR NOT NULL,
				PlaceId INTEGER REFERENCES fvaPlaces (ID) ON DELETE RESTRICT MATCH SIMPLE,
				People		VARCHAR,
				DevId INTEGER NOT NULL REFERENCES fvaDevices (ID) ON DELETE RESTRICT MATCH SIMPLE,
                                Description	VARCHAR,
				ScanerId INTEGER REFERENCES fvaDevices (ID) ON DELETE RESTRICT MATCH SIMPLE,
                                Comment		VARCHAR,
				OldName		VARCHAR,
				WhoTook		INTEGER REFERENCES fvaPeople (ID) ON DELETE RESTRICT MATCH SIMPLE,
				OldName1	VARCHAR 
                                );
.separator ,
.import "../../fva/#data#/fvaFile.csv" fvaFile
