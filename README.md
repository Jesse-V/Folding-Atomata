#Folding Atomata
###Alternative protein viewer for Folding@home

View the biological machines that run the body! Folding Atomata is a third-party simulation and protein viewer for [Folding@home](http://folding.stanford.edu/home/), a distributed computing project from Stanford University. Atomata aims to be a drop-in replacement for FAHViewer, the official viewer for F@h.

### System requirements

1. OpenGL 2.1, (GLSL 1.20) released in 2006. This is supported by Nouveau drivers.
2. The Linux operating system. Currently only supporting Mint, *buntu, and Debian.

### Installation

There are several ways to install Folding Atomata:

* **Installation from PPA. This is the recommended, easiest, and most secure route.** Simply run these commands:
> 1. **sudo add-apt-repository ppa:jvictors/gitstable**
> 2. **sudo apt-get update**
> 3. **sudo apt-get install folding-atomata**

    This will add my Personal Package Archive (PPA) to your sources, refresh the list of available packages, and then install Folding Atomata. Digital signatures and hashsums ensure integrity during the entire publishing process and during installation. Once installed, Atomata can then be managed, auto-updated, and uninstalled just like any other Linux package.

* Installation from .deb file. This is for Debian systems or if you don't want Atomata to automatically update.
> 1. Visit the [Releases page](https://github.com/Jesse-V/Folding-Atomata/releases).
> 2. Download the .deb folder from the latest release (the one at the top). i386 is 32-bit, amd64 is 64-bit.
> 3. Install the .deb file package.

    That .deb folder is hosted by Github. If you prefer, you can download the one from Launchpad by visiting [my PPA](https://launchpad.net/~jvictors/+archive/gitstable/+packages), clicking on the latest version build for your distribution, and then downloading and installing the appropriate .deb file.

* Install from source, for the latest and greatest!

    There's also the option of installing Folding Atomata directly from the source repository. I recommend visiting the [Releases page](https://github.com/Jesse-V/Folding-Atomata/releases) and downloading the .zip source of the latest stable edition. If you really want the cutting-edge and possible unstable edition, download the .zip from the top of the source tree via [this link](https://github.com/Jesse-V/Folding-Atomata/archive/master.zip).

    > 1. Once you have download the source, use the .gitInstall.sh script to install build, compile, and install the program. You will need the root password for this, because that script installs the libglew-dev, freeglut3-dev, and cmake libraries and installs to /usr/share/.
    > 2. If you don't want to install Atomata from the source, install those libraries via **sudo apt-get install libglew-dev freeglut3-dev cmake build-essential**, navigate into the _src_ directory, and then use the **./ClangCompileRun.sh** script to compile with the [Clang compiler](https://en.wikipedia.org/wiki/Clang), or alternatively, use **./compileRun.sh** to compile using GCC. I prefer using Clang for development, but it doesn't matter too much if you're installing it from source.

### Usage

Folding Atomata aims to be as simple as FAHViewer, but with more capabilities and excitement. Atomata installs into Menu => Education just like FAHViewer, and has it's own [logo](https://en.wikipedia.org/wiki/File:Protein_fold.png). Launching the program is easy: simply launch it from the Menu, or you can open the Terminal and type **FoldingAtomata** and it will run with verbose output. If you need help, type **man FoldingAtomata**. If you need additional help, send me a message over [Freenode's](http://webchat.freenode.net/) #folding@home IRC channel, over [foldingforum.org](http://foldingforum.org/), or via email.

The camera is controlled via the standard game keybindings: W and S go forward and backward respectively, A and D moves left and right, and Q and E moves up and down. You can look around using the mouse. These are ordinary controls used in many games, including Minecraft. Your motion through space is supposed to be fluid and smooth, so enjoy and don't forget to look around as you're moving!

The animation moves from one available snapshot to the other, and then starts over when it reaches the end. This is in contrast to FAHViewer, which runs the animation backwards when it reaches the last snapshot. Currently, Atomata is unable to render new snapshots as they come in, but FAHViewer can. I intend to fix this.

### What does "Folding Atomata" mean?

It's a play on words, almost a portmanteau. The root is "automaton", (plural automata) which is a self-operating machine. These can be a full-sized machine or robot, or self-operating process such as a [cellular automaton](http://en.wikipedia.org/wiki/Cellular_automaton). The term "automaton" also refers to [finite-state machines](http://en.wikipedia.org/wiki/Finite-state_machine). These are often used to describe simple computer pgorams or electronic/logical processes. Since "Atomata" has the prefix "atom," the name "Folding Atomata" turns out to be very applicable.

All of this makes sense in the context of this program. Proteins are self-operating machines: they are the workhorses in biology, play essential roles in nearly all biological functions and processes, and bind together to create a ribosome and create more of themselves. Yet they must first fold themselves into a functional three-dimensional structure before they can take on any of these roles. As a protein folds, [it transitions](http://en.wikipedia.org/wiki/File:ACBP_MSM_from_Folding@home.tiff) from one intermediate semi-stable fold to another until it finally reaches its stable native state. A protein can spend upwards of 96% of its folding time in these intermediate states, so scientists often model protein folding using an [energy function and Markov State Models](http://folding.stanford.edu/home/faq/faq-simulation/). MSMs can be represented as finite-state machines, except that the transitions are probabilistic/stochastic rather than deterministic. MSMs are a key component to Folding@home: it is because of them that distributing computing is even an option for simulating protein folding. (Each workunit is a piece of this model.) This program animates and explores the atoms of the protein as the computer processes it. So overall, "Folding Atomata" fits with the many aspects within the scope of the program.

### Reporting bugs and the general to-do list

Encountered a bug? Would you like to help improve Atomata by offering suggestions? Interested in seeing what enhancements I have planned? The [Issues page](https://github.com/Jesse-V/Folding-Atomata/issues) holds all of this information. If you have a Github account, filing a ticket is really easy. Detailed descriptions are very helpful when submitting. If it's a bug, feel free to attach a screenshot. If you don't have a Github account, making one is extremely easy, but if you're not interested, send me a message over IRC, foldingforum.org, or by email and I may end up filing the ticket for you. I'm only working on this part-time, and I'm at college, but I'd like to get to all the tickets when I have time!

### Development

The _master_ branch is the unstable development edition. I usually push compilable versions, but not all the time. I also prefer to make small commits, so features in the master branch aren't guaranteed to be complete or work as intended. When I've reached a stable point, I'll tag the commit, (using a 2048-bit digital signature) create a Github release, and push to my Launchpad PPA. These releases generally follow the [milestones](https://github.com/Jesse-V/Folding-Atomata/issues/milestones) and the completeness of their corresponding issues. I try to follow [semantic versioning](http://semver.org/) so the version format is major.minor.patch.build. 

When developing, the *clean.sh* script in the _src_ directory is useful for cleaning out the files generated by CMake when it builds and compiles the code. Since this process is dependent on the working directory and environment, it makes sense to me to run this script to clean the build environment before I push to Github.

Wherever reasonably possible, the programming style strives to follow http://geosoft.no/development/cppstyle.html with the exception of #85.

#### Porting to Windows

For time reasons, I only want to support Linux at the moment. However, porting Folding Atomata to Windows should not be difficult. CMake should generate a decent Visual Studio project, and I have a pretty good idea how to get it to look for the relevent libraries. Glew and Glut are very popular, they're easy to install in Windows, there's plenty of documentation out there, and compilation should be straightforward. I'm particularly interested in building a .exe or .msi installer, so if anyone is up for the fun challenge I'd be willing to support them however I can, and they'd get to have their name on it too!

### Origin

This project inherited the rendering and game engine developed for [iVoxely](https://github.com/Jesse-V/iVoxely), but I made improvements to the engines while developing Atomata.
