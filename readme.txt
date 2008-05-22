gen_cross_section
=================

Introduction
------------

gen_cross_section is a tool for creating PNG images of chip layout cross
sections. Layout tools use a plan view but sometimes it is useful to be able
to look at the cross section of a particular part of a design.
It doesn't provide a true cross section because it assumes that all layers
are perfectly planar.


Getting Started
---------------

You will need to create/modify two files to match your process - palette.txt
and layers.txt. Examples of these files are provided for a generic p-sub 4
metal layer process are included.

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

Customise genCrossSection() in cross_section.il (the paths, the grid and the
size of the image) and then use:

load("/path/to/cross_section.il")
genCrossSection()

There's also a skill file for adding a menu to the layout editor.

Users of other software will have to write their own scripts to generate the
input file. I'd be very happy to receive code for other software.


Cadence Skill Code
------------------

The functions XyzGetLppsAtPoint and XyzGetOvlpObj, which do most of the work 
getting the layer information, were written by Jim Newton, part of the Cadence 
VCAD team. As such, Jim has given me permission to distribute the code but
that does mean it is not covered by the license in COPYING.txt.

The original thread where Jim provided the functions can be seen at:

http://groups.google.com/group/comp.cad.cadence/browse_thread/thread/503a417a2bc94e33/c12d840b2066e820

The Cadence VCAD (Virtual CAD) team provide, amongst other things, high
quality SKILL IP to help users get around various shortcomings in the Cadence
tools. More information on Cadence VCAD can be found at:

http://www.cadence-europe.com/solutions/vpage.cfm


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
http://atchoo.org/tools/gen_cross_section/

