<p align="center">
  <a href="FVAIcons/main.png" target="blank"><img src="FVAIcons/main.png" width="120" alt="FVA Logo" /></a>
</p>
<p align="center">
  <img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/dimanikulin/fva?style=social">
  <img alt="GitHub contributors" src="https://img.shields.io/github/contributors/dimanikulin/fva">
  <img alt="GitHub followers" src="https://img.shields.io/github/followers/dimanikulin?style=social">
  <img alt="GitHub Sponsors" src="https://img.shields.io/github/sponsors/dimanikulin">
  <img alt="Twitter URL" src="https://img.shields.io/twitter/url?style=social&url=https%3A%2F%2Ftwitter.com%2FDmytroNikulin" href="https%3A%2F%2Ftwitter.com%2FDmytroNikulin">
  </br>
  <img alt="GitHub release (latest by date including pre-releases)" src="https://img.shields.io/github/v/release/dimanikulin/fva?include_prereleases">
  <img alt="GitHub all releases" src="https://img.shields.io/github/downloads/dimanikulin/fva/total">
  <img alt="GitHub Release Date" src="https://img.shields.io/github/release-date-pre/dimanikulin/fva">
  </br>
  <a href="https://github.com/dimanikulin/fva/actions/workflows/main.yml"><img src="https://github.com/dimanikulin/fva/actions/workflows/main.yml/badge.svg?branch=master" alt="Tests"/></a>
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/dimanikulin/fva">
  <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/dimanikulin/fva">  
  <a href="https://www.codefactor.io/repository/github/dimanikulin/fva"><img src="https://www.codefactor.io/repository/github/dimanikulin/fva/badge" alt="CodeFactor" /></a>
  </br>
  <img alt="GitHub issues" src="https://img.shields.io/github/issues/dimanikulin/fva">
  <img alt="GitHub closed issues" src="https://img.shields.io/github/issues-closed/dimanikulin/fva">
  <img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/dimanikulin/fva">
  <img alt="GitHub closed pull requests" src="https://img.shields.io/github/issues-pr-closed/dimanikulin/fva">
  </br>
  <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/dimanikulin/fva">
  <img alt="GitHub language count" src="https://img.shields.io/github/languages/count/dimanikulin/fva">
  <img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/dimanikulin/fva">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/dimanikulin/fva">
  </br>
  <a href="https://github.com/dimanikulin/fva/projects/4"><img src="https://img.shields.io/badge/roadmap-view-d90368"> </a>
  <img alt="GitHub Discussions" src="https://img.shields.io/github/discussions/dimanikulin/fva">
  <img alt="GitHub" src="https://img.shields.io/github/license/dimanikulin/fva">
  <a href="https://github.com/dimanikulin/fva/edit/master/README.md"><img src="https://img.shields.io/badge/documentation-read-d90368"> </a>
</p>

# Description
FVA Software (Photo-Video-Audio Software) is a convergent tool set in a form of desktop applications aimed at automating importing process of [Multimedia Data](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaData.md) (photo/audio/video)
into [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md) like [digiKam](https://www.digikam.org/) 
<img src="./img_digiKam.png" alt="digiKam.png" width="20" height="20"/>,
 Google Photo, Apple Photo, [Synology Moments](https://www.synology.com/en-global/dsm/feature/moments) 
<img src="./img_SynologyMoments.png" alt="SynologyMoments.png" height="20" />, 
etc. to make multimedia more fully-searchable as well as to refine family photos.
<img src="./img_InputAndOutputExamples.gif" alt="Input And Output Examples" />

# Capabilities
- **Organizing the multimedia:** Organizing the [Multimedia Data](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaData.md) to be more searchable
- **Multimedia metadata gaps identification:** It identifies the gaps and tries to fill it up (enriching the metadata and improving its consistency with minimal human involvement).
- **Integration:** The system is integrated with online/offline [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md):
[digiKam](https://www.digikam.org/) <img src="./img_digiKam.png" alt="digiKam.png" width="20" height="20"/> as Offline [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md) and 
[Synology Moments](https://www.synology.com/en-global/dsm/feature/moments) <img src="./img_SynologyMoments.png" alt="SynologyMoments.png" height="20" /> as Online [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md) are now supported.
- **Supporting text search for [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md):** It is possible to search by events, photo author, places, people using text search 
- **Configuration:** The system flow depends on configuration.
- **Automating the import:** to decrease the probability of human factor influence on the process of importing new photos into the existing photo album;
Despite many steps are automated, still human involvement is required. 

# Software Architecture Requirements

## Constraints
This section lists constrains and explanations for them. 

| # | Constraint  | Description | Business value | Architecture viewpoint |
| - | ------------------------|----|----|:--------------:|
| 1 | The FVA software shall be installed on the same environment that Multimedia IR system is already working on. | Not to implement the network interaction with another environment | M | L |
| 2 | The same photo album storage shall be used for offline or online Multimedia IR system and the FVA software. | Not to implement the interaction to another storage | M | L |
| 3 | Target FVA file name shall be unique for a directory it is kept in (for any directory in the album). If several photos were taken at one time the time shift shall be applied for them. | It simplifies implementation of solution | L | H |
| 4 | The taken time that is kept in file name has the highest priority comparing for taken time in EXIF or file modification time. | A lot of already renamed files that are now in photo album have the EXIF date is empty as well as file modification time.(It is only applicable for the author photo album) | L | L |
| 5 | One format for pictures should be used (jpeg) as a widely used with EXIF support, any other format must be converted to this one with adding EXIF when needed | It simplifies implementation of solution | L | H |
| 6 | One folder shall keep multimedia data for one event only | It simplifies organization structure of photo album, implementation of solution and make photo album more searchable  | L | H |
| 7 | The solution shall use open source tools only | As it is going to be open source as well | H | M |

## Assumptions
This section lists all the assumptions with explanations for them. 
This is critical to do in order to cover all the gaps in the requirements.

| # | Assumption Description  | 
| - |:--------------:|
| 1 | It is assumed that the user, which the proposed solution is going to run under, has the write access to the [Google Photo](https://en.wikipedia.org/wiki/Google_Photos) (and other IR Multimedia systems) folder where [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) processed is going to be put in. | 
| 2 | User restarted the wizard after changing the configuration (because of feature of QTWizard and QTWizardPage) |

## Quality Attributes
This section lists quality attributes for target architecture.
So there are following Quality Attributes: **Conceptual Integrity**, **Maintainability**, **Reusability**, **Availability**, **Interoperability**, **Manageability**, **Performance**, **Reliability**, **Scalability**, **Security**, **Portability**, **Supportability**, **Testability**, **Learnability**, **Traceability**.

For more details please [see](./QualityAttributes.md)

## Hardware requirements
TBD

# Software Architecture Views

## Context View
The context diagram below shows the solution bounds and its interactions with other objects and subjects.

```mermaid
flowchart LR
    B -->|Multimedia data| C(Online Multimedia IR system)
    B -->|Multimedia data| D(Offline Multimedia IR system)
    A(Photo Album owner) --> B(FVA Solution)
    A(Photo Album owner) --> D1
    A(Photo Album owner) --> D2
    A(Photo Album owner) --> D3
    D1(fa:fa-tablet Tablet) -->|Multimedia data| B(FVA Solution)
    D2(fa:fa-phone Phone) -->|Multimedia data| B(FVA Solution)
    D3(fa:fa-hdd-o External Disk) -->|Multimedia data| B(FVA Solution)
```
The solution is used during import of new multimedia content from external devices to an existing user photo album. 

The photo album owner is the main and only user of the solution. 

The external devices might be: 
- the phones with a camera; 
- the tablets with a camera; 
- the photo/video cameras;
- external storages (e.g., hard drive or flash card) with multimedia data;
- etc.

Once the multimedia data from the external devices is processed, the solution interacts with integrated online and offline Multimedia systems.

## Functional View

The Layered Architecture shall be applied to the system. 
Following layers are considered:

```mermaid
flowchart BT
    A(UI) --> B(Processing)
    B(Processing) --> C(Data and Configuration)
```
For more details please [see](./FunctionalView.md)

## Deployment View
The target installation folder shall have next folder structure:
* **#bin#** - a directory with all binaries (product one either third party);
* **#data#** - a directory to keep user populated dictionaries and application data;
* **#logs#** - a directory to keep the application logs. Each log file name shall start from name of process it was created by;
* **#scripts#** - a directory to keep all the scripts.

Installation must be possible at any target directory but not only in root folder.

TBD describe dependencies installed along with FVA Software.

TBD fvaParams.csv

# Programming Languages                         
Now C++ and Python are considered as main languages to implement FVA Software.

TBD - to describe for which cases C++ and for which Python.

# License
we use 3-rd party code here TBD link
and own licence is TBD

# Dependencies
Only open source [code/tools](./Dependencies.md) are used, as a product is **open source**.

There were other [dependencies](./DependenciesOld.md) but they got outdated.

# Contributing
So far, the development is in a hot stage, and everyone is welcome to contribute anytime.
You can fill in this [form](https://docs.google.com/forms/d/1EI3oOumRnHxDjEYgV6PFB-AMfV5plLtuV2r5S8BYn_g/edit) to understand your needs in Multimedia search and organizing.
You can also post an issue here [GitHub issues](https://github.com/dimanikulin/fva/issues) to request a feature or report a bug.
[Here](https://github.com/dimanikulin/fva/discussions) you can start a discussion.

Everyone can try themselves in different roles to learn something new: 
- [As a product manager](./FVADocMD/LEARNT_AS_PM.md);
- [As a delivery manager](./FVADocMD/LEARNT_AS_DM.md);
- [As an application architect](./FVADocMD/LEARNT_AS_SA.md); 
- [As a software developer](./FVADocMD/LEARNED_AS_DEV.md).
- [As a subject matter expert](./FVADocMD/LEARNT_AS_SME.md). 
- [As a software tester you can download installation packages](https://github.com/dimanikulin/fva/releases) that are published for Linux (x86-64), Mac and Windows (latest versions) to try it.

So, there are many items to help the author with.
  
# Stay in touch
* [Twitter](https://twitter.com/DmytroNikulin)
* [LinkedIn](https://www.linkedin.com/in/dimanikulin/)
* [Author Telegram](https://t.me/dimanikulin79) 
* [Community Telegram](https://t.me/+XqWdomi90-00Yzgy) 

# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [Exif](https://en.wikipedia.org/wiki/Exif)| Exchangeable image file [format](https://en.wikipedia.org/wiki/File_format) (officially Exif, according to JEIDA/JEITA/CIPA specifications) is a standard that specifies the [formats](https://en.wikipedia.org/wiki/File_format) for [images](https://en.wikipedia.org/wiki/Image), [sound](https://en.wikipedia.org/wiki/Sound), and ancillary tags used by [digital cameras](https://en.wikipedia.org/wiki/Digital_camera) (including [smartphones](https://en.wikipedia.org/wiki/Smartphone)), [scanners](https://en.wikipedia.org/wiki/Image_scanner) and other systems handling image and sound files recorded by digital cameras.|
| 2 | [Google](https://en.wikipedia.org/wiki/Google)| Google LLC is an American multinational technology company that specializes in Internet-related services and products, which include [online advertising technologies](https://en.wikipedia.org/wiki/Online_advertising), a [search engine](https://en.wikipedia.org/wiki/Search_engine), [cloud computing](https://en.wikipedia.org/wiki/Cloud_computing), software, and hardware.|
| 3 | IR Multimedia Systems |multimedia information retrieval means the process of searching for and finding multimedia documents; the corresponding research field is concerned with building multimedia search engines. |
| 4 | [Multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008)| Multimedia in principle means data of more than one medium. It usually refers to data representing multiple types of medium to capture information and experiences related to objects and events. Commonly used forms of data are numbers, alphanumeric, text, images, audio, and video. In common usage, people refer a data set as multimedia only when time-dependent data such as audio and video are involved.|
| 5 | Multimedia IR| Multimedia Information Retrieval (MIR) is an organic system made up of Text Retrieval (TR); Visual Retrieval (VR); Video Retrieval (VDR); and Audio Retrieval (AR) systems. So that each type of digital document may be analyzed and searched by the elements of language appropriate to its nature, search criteria must be extended.|
