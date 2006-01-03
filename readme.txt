gen_cross_section 0.1
=====================

Introduction
------------

gen_cross_section is a tool for creating PNG images of chip layout cross
sections. Layout tools use a plan view but sometimes it is useful to be able
to look at the cross section of a particular part of a design.

Getting Started
---------------

You will need to create/modify two files to match your process - palette.txt
and layers.txt.

Palette.txt defines the colours used to draw the cross section and layers.txt
defines how these layers are drawn. The examples provided are for a p-sub,
single well, single poly and four metal process.

You will then run the program with your input file:

gen_cross_section -i input.txt -o output.png -p palette.txt -l layers.txt

See

gen_cross_section -h

for more options.

Generating the Input File
-------------------------

Cadence users should look in the cadence directory for the cross_section.il
file which contains Skill code for generating the appropriate input file.
Customise gen_CrossSection() in cross_section.il (the paths, the grid and the
size of the image) and then use:

load("/path/to/cross_section.il")
genCrossSection()

Users of other software will have to write their own scripts to generate the
input file. I'd be very happy to receive code for other software.

Input File Format
-----------------

The input file is very simple.

The first line is an integer showing the number of points in the cross
section.
After that, a new cross section point is started by a line consisting of a
full stop (.). All of the layers that are present in that point are listed,
one to a line. The point is finished with a blank line.

See example.txt for an example cross section file.

Contact
-------

gen_cross_section was written by Roger Light, with the important part of the
Cadence Skill code being written by Jimka.

Questions, bug reports, suggestions or request are very welcome.

Contact me on roger@atchoo.org

Updates to gen_cross_section should be at
http://www.atchoo.org/chiptools/gen_cross_section/
