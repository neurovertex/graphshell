GraphShell {#mainpage}
==========

This is an attempt at a general-purpose shell with GUI designed to pipe data between programs as easily as possible. Basically the same thing you do in a command-line shell when you use IO redirection (|, >, < ...), but hopefully better.

Wut
---

This was inspired from various software, like [Coregraphe](http://doc.aldebaran.com/1-14/software/choregraphe/) (drag'n'drop GUI to program NAO robots), [OpenViBE](http://openvibe.inria.fr/) (Signal processing software for EEG).

Here's an example: You have a file with newline-separated links and want to download every file. In bash, that would be something like ```cat links.txt | (while read link; do wget "$link"; done) |& sed -rne 's/^.*‘(.*)’.*saved.*$/\1/p'``` (wget outputs to stderr)

![Example1](https://raw.githubusercontent.com/neurovertex/graphshell/master/doc/Example1.png)

We can see three kind of elements here :
* Boxes, which can do anything you want, really.
* Sockets (I/O endpoints), which have a type (text, image, ...), and a name (like stderr/stdout for a box that'd execute an external tool). Stream types are a continuous/contiguous stream of data while signals are individual objects/entities.
* Pipes. They link stuff.

So that's supposed to do the same thing as the command, so what's the point ? Well here's non-exhaustive list of thing it adds:
* Ever tried to do non-linear piping chain (like splitting the pipeline in two or something) in a shell ? It's a pain, because it's a command-*line* interface, thus it's by nature linear. GUI don't have that limitation.
* Arbitrary number of inputs/outputs with actual typing. It's true text streams are a universal interface, but sometimes being able to send more complex kinds of data can be cool too.
* Hot-Swappable pipes! Change the redirections while your stuff is running. You definitively can't do that (*easily*)  in a CLI.
* Ability to save complex configuration

At the moment the project is barely in a sketch state. Planned feature include an optional GUI (If possible, a remote one), a python API to use

Goal
----

The purpose of this is not to replace shells altogether of course. It is to implement a new kind of script to run stuff that need to exchange data easily, trying to port the ease of piping I/O in a shell to a graphical program.

-----------------

[![Stories in Ready](https://badge.waffle.io/neurovertex/graphshell.png?label=ready&title=Ready)](http://waffle.io/neurovertex/graphshell)
