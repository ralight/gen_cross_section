/* These functions add a menu to the layout tool
 * for creating cross section drawings.
 *
 * To use, add the following lines to your .cdsinit
 *
 * load("/path/to/layout_menu.il")
 * load("/path/to/cross_section.il")
 * deRegUserTriggers("maskLayout" nil nil 'myLayoutTrigger)
 *
 * You also need to customise the paths in the createOtherMenu()
 * calls to GetCrossSection() to match your own installation.
 */

procedure(createOtherMenu( )
	itemCrossSect = hiCreateMenuItem(
		?itemText "Generate Cross Section"
		?callback "GenCrossSection(\"/path/to/layers.txt\" \"/path/to/palette.txt\" 800)"
		?name 'itemCrossSect)
	
	itemCrossSectScale = hiCreateMenuItem(
		?itemText "Generate Cross Section (to scale)"
		?callback "GenCrossSection(\"/path/to/layers_scale.txt\" \"/path/to/palette.txt\" 800)"
		?name 'itemCrossSectScale)
	
	rogerMenu = hiCreatePulldownMenu(
		'otherMenu
		"Other"
		list(itemCrossSect itemCrossSectScale))
)

procedure(otherMenuTrigger( args )
	createOtherMenu()

	hiInsertBannerMenu(
		hiGetCurrentWindow()
		otherMenu
		length(hiGetBannerMenus( hiGetCurrentWindow() ) )
		)
)

procedure( myLayoutTrigger( l )
	otherMenuTrigger( l)
)

