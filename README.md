#Folding Atomata
###Alternative protein viewer for Folding@home

View the biological machines that run the body! Folding Atomata is a third-party simulation and protein viewer for [Folding@home](http://folding.stanford.edu/home/), a distributed computing project from Stanford University. Atomata aims to be a drop-in replacement for FAHViewer, the official viewer for F@h.

Here are some screenshots of it in action. This is an example view of the demo protein:
![](https://f.cloud.github.com/assets/2314417/1374232/426ebbba-3a6e-11e3-9df8-45e1dfa34cf3.png "FoldingAtomata displaying a colorful ball-and-stick model of an protein. The protein is is bounded by a faint green wire mesh, and there is a faint square point gradient in the skybox in the background.")

Here is a close-up of one section of the protein. Following the [CPK coloring scheme](https://en.wikipedia.org/wiki/CPK_coloring), hydrogen atoms are white, carbons are dark gray, and nitrogen is sky-blue. Not shown here is oxygen (red) and sulfur (yellow). The comprise all the elements that are likely to be found in a protein. All other elements are colored orange.
![](https://f.cloud.github.com/assets/2314417/1374234/449f620e-3a6e-11e3-9981-a60f157340f4.png "A close-up of some of the atoms.")

This is a neat view of a what appears to be a [imidazole](https://en.wikipedia.org/wiki/Imidazole), an unsaturated [heterocyclic](https://en.wikipedia.org/wiki/Heterocyclic) [carbon ring](https://en.wikipedia.org/wiki/Carbon_ring). This can be found on the side of the demo protein.

![](https://f.cloud.github.com/assets/2314417/1374490/943fbe28-3a71-11e3-8c1b-807d65256b31.png "A close view of an unsaturated heterocyclic carbon ring.")

Atomata animates the protein using available checkpoints, allowing you to see an approximation of how the protein move and morphs within the timeframe of your workunit. Unlike FAHViewer, with Folding Atomata you have ability to fly all around the protein. Back off for the bigger picture, or get in close for a more detailed look.

Since it's common for a computer to work on two different workunits simultaneously, Atomata will illustrate this by rendering the proteins from all available slots from a running client. (Note that this will not include FahCore_17 slots.)

### System requirements

1. The Linux operating system.
2. OpenGL 2.1 (GLSL 1.20)

Note that the Folding@home software is not required per se, as Atomata will use a demo protein as a fallback.

OpenGL 2.1 was released in 2006 and is supported by the default open-source Nouveau drivers, but proprietary vendor-specific drivers should also work as well. I provide packages for Mint, Ubuntu, and Debian, so installation and management is easiest for them. For all other Linux distributions, Atomata can be installed from the source through my instructions below. All of these options should be straightforward.

### Installation

Once you have Folding@home installed and running, you're ready to install Atomata. There are several ways to do this:

* **Installation from PPA. This is the recommended, easiest, and most secure route, and applies to Ubuntu and [all derivatives](http://tinyurl.com/ubuntu-derivatives) (including Kubuntu, Xubuntu, and Mint).** Simply run these commands:
> 1. **sudo add-apt-repository ppa:jvictors/gitstable**
> 2. **sudo apt-get update**
> 3. **sudo apt-get install folding-atomata**

    This will add my Personal Package Archive (PPA) to your sources, refresh the list of available packages, and then install Folding Atomata. Digital signatures and hashsums ensure integrity during the entire publishing process and during installation. Once installed, Atomata can then be managed, auto-updated, and uninstalled just like any other Linux package. This makes it easy to stay updated on the latest releases.

* Installation from .deb file. This is for Debian systems or if you don't want Atomata to automatically update.
> 1. Visit the [Releases page](https://github.com/Jesse-V/Folding-Atomata/releases).
> 2. Download the .deb file for the latest release. i386 is 32-bit, amd64 is 64-bit.
> 3. Install the .deb package.

    Those .deb files are hosted by Github. If you prefer, you can download the one from Launchpad by visiting [my PPA](https://launchpad.net/~jvictors/+archive/gitstable/+packages), clicking on the latest version build for your distribution, and then downloading and installing the appropriate .deb file.

* Install from source, for the latest and greatest!

    There's also the option of installing Folding Atomata directly from the source repository. I recommend visiting the [Releases page](https://github.com/Jesse-V/Folding-Atomata/releases) and downloading the .zip source of the latest stable edition. If you really want the cutting-edge and possible unstable edition, download the .zip from the top of the source tree via [this link](https://github.com/Jesse-V/Folding-Atomata/archive/master.zip).

    > 1. Once you have download the source, you will need the libglew-dev, freeglut3-dev, libpng-dev, cmake, and build-essential libraries.
    > 2. Navigate into the _src_ directory, and then run the following:
    > 3. **cp -rl ../debian/ debian/ && cd debian/extra_includes/ && gzip --best -c manpage > FoldingAtomata.1.gz && cd ../../**
    > 4. **cmake . && make -j 4** and then as root run **make install**

    You can also use **./ClangCompileRun.sh** in the _src_ directory to compile with the [Clang compiler](https://en.wikipedia.org/wiki/Clang), or alternatively, use **./compileRun.sh** to compile using GCC. I prefer using Clang for development, but it doesn't matter too much if you're installing it from source.

### Usage

Folding Atomata aims to be as simple as FAHViewer, but with more capabilities and excitement. Atomata installs into Menu => Education just like FAHViewer, and has it's own [logo](https://en.wikipedia.org/wiki/File:Protein_fold.png). Launching the program is easy: simply launch it from the Menu, or you can open the Terminal and type **FoldingAtomata**. If you need help, type **man FoldingAtomata** or **FoldingAtomata --help**. If you need additional help, send me a message over [Freenode's](http://webchat.freenode.net/) #folding@home IRC channel, over [foldingforum.org](https://foldingforum.org/ucp.php?i=pm&mode=compose&u=18404), or via email.

The camera is controlled via the standard game WSAD keybindings: W and S go forward and backward respectively, A and D moves left and right, and Q and E moves up and down. You can look around using the mouse. These are ordinary controls used in many games, including Minecraft. Your motion through space is supposed to be fluid and smooth and will slow to a halt over time, so enjoy and don't forget to look around as you're moving!

The animation is simple linear interpolate between pairs of snapshots. By default, the animation will start over when it reaches the end, thus making it easy to distinguish the correct direction the protein is moving as you are processing it. This is in contrast to FAHViewer, which runs the animation backwards when it reaches the last snapshot. If you prefer this behavior, see the --cycle-snapshots flag in the list below. Providing it will change the animation to follow FAHViewer. Currently, Atomata is unable to render new snapshots as they come in, but FAHViewer can. I intend to fix this.

### Flags

Like FAHViewer, an easy way to control the program is through command-line flags, and this is a common theme for Linux applications anyway. The most important flags are given by FAHControl, and Atomata can handle many of these. The list of flags are:

    --animationDelay, -ad  Milliseconds to wait between each animation frame.
    --connect, -c          Address and port to use to connect to FAHClient.
    --cycle-snapshots      If enabled, the animation runs backwards at end.
    --help, -h             Show flag options and their usage.
    --image-a, -ia         Specifies the path to image A for the skybox.
    --image-b, -ib         Specifies the path to image B for the skybox.
    --image-c, -ic         Specifies the path to image C for the skybox.
    --license              Prints license information.
    --mode, -m             Rendering mode. 3 is stick. Ball-n-stick by default.
    --no-skybox            Disables the skybox, leaving a black background.
    --password, -p         Password for accessing the remote FAHClient.
    --slices, -sl          Slices to use for the atom mesh. Default is 8.
    --stacks, -st          Stacks to use for the atom mesh. Default is 16.
    --verbose, -v          Verbose printing to stdout.
    --version              Print version information.

For key-value flags, two formats are supported: "--<option> <value>" and "--<option>=<value>". For boolean values or for flags that don't have any values attached (e.g. --version) just provide the flag.

### Security

Computer and network security is becoming increasingly important, and it is something that interests me. I use the 2048-bit RSA key 0xC20BEC80 to digitally sign my files when pushing to Launchpad. A virtual machine then remotely compiles and builds the package from source within a fresh installation of Ubuntu. For a PPA install, the download is verified using a 1024-bit digital signature from Launchpad. For either a PPA or .deb install, the installation uses hash functions to confirm integrity. The code is also open-source, and you are free to email me if you have any questions or concerns regarding any of its contents.

That being said, Folding Atomata has exactly the same security weakness that FAHViewer has: the password for remote clients is transmitted in cleartext. There is little that either of these programs can do about that. There's also no authentication or encryption when connecting to remote clients, so that bit requires a bit of trust and responsibility.

### What does "Folding Atomata" mean?

It's a play on words, a sort of portmanteau. The root is "automaton", (plural automata) which is a self-operating machine. These can be a full-sized machine or robot, or self-operating process such as a [cellular automaton](http://en.wikipedia.org/wiki/Cellular_automaton). The term "automaton" also refers to [finite-state machines](http://en.wikipedia.org/wiki/Finite-state_machine). These are often used to describe simple computer pgorams or electronic/logical processes. Since "Atomata" has the prefix "atom," the name "Folding Atomata" turns out to be very applicable.

All of this makes sense in the context of this program. Proteins are self-operating machines: they are the workhorses in biology, play essential roles in nearly all biological functions and processes, and bind together to create a ribosome and create more of themselves. Yet they must first fold themselves into a functional three-dimensional structure before they can take on any of these roles. As a protein folds, [it transitions](http://en.wikipedia.org/wiki/File:ACBP_MSM_from_Folding@home.tiff) from one intermediate semi-stable fold to another until it finally reaches its stable native state. A protein can spend upwards of 96% of its folding time in these intermediate states, so scientists often model protein folding using an [energy function and Markov State Models](http://folding.stanford.edu/home/faq/faq-simulation/). MSMs can be represented as finite-state machines, except that the transitions are probabilistic/stochastic rather than deterministic. MSMs are a key component to Folding@home: it is because of them that distributing computing is even an option for simulating protein folding. (Each workunit is a piece of this model.) This program animates and explores the atoms of the protein as the computer processes it. So overall, "Folding Atomata" fits with the many aspects within the scope of the program.

### Reporting bugs and the general to-do list

Encountered a bug? Would you like to help improve Atomata by offering suggestions? Interested in seeing what enhancements I have planned? The [Issues page](https://github.com/Jesse-V/Folding-Atomata/issues) holds all of this information. If you have a Github account, filing a ticket is really easy. Detailed descriptions are very helpful when submitting. If it's a bug, feel free to attach a screenshot. If you don't have a Github account, making one is extremely easy, but if you're not interested, send me a message over IRC, foldingforum.org, or by email and I may end up filing the ticket for you. I'm only working on this part-time, and I'm at college, but I'd like to get to all the tickets when I have time!

### Development

The _master_ branch is the unstable development edition. I usually push compilable versions, but not all the time. I also prefer to make small commits, so features in the master branch aren't guaranteed to be complete or work as intended. When I've reached a stable point, I'll tag the commit, (using a 2048-bit digital signature) create a Github release, and push to my Launchpad PPA. These releases generally follow the [milestones](https://github.com/Jesse-V/Folding-Atomata/issues/milestones) and the completeness of their corresponding issues. I try to follow [semantic versioning](http://semver.org/) so the version format is major.minor.patch.build.

When developing, the **clean.sh** script in the _src_ directory is useful for cleaning out the files generated by CMake when it builds and compiles the code. Since this process is dependent on the working directory and environment, it makes sense to me to run this script to clean the build environment before I push to Github.

Wherever reasonably possible, the programming style strives to follow http://geosoft.no/development/cppstyle.html with the exception of #85.

#### Porting to Windows/OS-X

I don't currently offer a release of Folding Atomata for Windows. However, porting it would not be difficult, as Glew, Freeglut, and PNG are very popular and should not be difficult to install. After upgrading Visual Studio's compiler so that it can handle modern C++11 elements like initializer lists, two elements in the code will have to be changed: the Socket class which currently uses Unix sockets, and main.cpp currently redirects standard output into /dev/null if the verbosity flag isn't provided, and /dev/null doesn't exist in Windows. I have provided some URLs in the code that may be of use to anyone wanting to make these changes. A release for Windows would also require a .exe or .msi installer, which I have no experience with. First one to offer a port gets their name on it!

A port to OS X is unlikely to happen. I don't have a Mac that I can use for testing. However, I will support anyone who wants to port the code.

If anyone is interested in porting, feel free to just fork the repository and code away. :)

### Origin

This project inherited the rendering and game engine developed for [iVoxely](https://github.com/Jesse-V/iVoxely), but I've made some significant improvements and general changes to the rendering and game engine while developing Atomata.
