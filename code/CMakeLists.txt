# CMakeList.txt : CMake project for Red Alert 
cmake_minimum_required (VERSION 3.8)

project(RedAlert CXX C)
enable_language(ASM)
set(CMAKE_CXX_FLAGS "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(src_io
	./redalert/io/userinput.cpp
	./redalert/io/userinput.h
)

set(src_win32lib
	./redalert/win32lib/tobuff.cpp
	./redalert/win32lib/alloc.cpp
	./redalert/win32lib/audio.h
	./redalert/win32lib/buffer.cpp
	./redalert/win32lib/buffer.h
	./redalert/win32lib/buffglbl.cpp
	./redalert/win32lib/defines.h
	./redalert/win32lib/delay.cpp
	./redalert/win32lib/descmgmt.h
	./redalert/win32lib/difftb.inc
	./redalert/win32lib/dipthong.cpp
	./redalert/win32lib/dipthong.h
	./redalert/win32lib/dplay.h
	./redalert/win32lib/drawbuff.h
	./redalert/win32lib/drawbuff.inc
	./redalert/win32lib/drawmisc.cpp
	./redalert/win32lib/drawrect.cpp
	./redalert/win32lib/dsetup.h
	./redalert/win32lib/dsound.h
	./redalert/win32lib/externs.h
	./redalert/win32lib/fastfile.h
	./redalert/win32lib/file.h
	./redalert/win32lib/filepcx.h
	./redalert/win32lib/filetemp.h
	./redalert/win32lib/font.h
	./redalert/win32lib/function.h
	./redalert/win32lib/gbuffer.cpp
	./redalert/win32lib/gbuffer.h
	./redalert/win32lib/gbuffer.inc
	./redalert/win32lib/getshape.cpp
	./redalert/win32lib/iconcach.h
	./redalert/win32lib/iconset.cpp
	./redalert/win32lib/iff.cpp
	./redalert/win32lib/iff.h
	./redalert/win32lib/indextb.inc
	./redalert/win32lib/irandom.cpp
	./redalert/win32lib/keyboard.h
	./redalert/win32lib/keyboard.inc
	./redalert/win32lib/keystruc.inc
	./redalert/win32lib/load.cpp
	./redalert/win32lib/loadfont.cpp
	./redalert/win32lib/mcgaprim.inc
	./redalert/win32lib/memflag.h
	./redalert/win32lib/misc.h
	./redalert/win32lib/modemreg.h
	./redalert/win32lib/mono.h
	./redalert/win32lib/mouse.h
	./redalert/win32lib/mouse.inc
	./redalert/win32lib/mouseshape.cpp
	./redalert/win32lib/mouseshape.h
	./redalert/win32lib/mouseww.cpp
	./redalert/win32lib/nybbtb.inc
	./redalert/win32lib/palette.cpp
	./redalert/win32lib/palette.h
	./redalert/win32lib/playcd.h
	./redalert/win32lib/profile.h
	./redalert/win32lib/profile.inc
	./redalert/win32lib/rawfile.h
	./redalert/win32lib/shape.h
	./redalert/win32lib/shape.inc
	./redalert/win32lib/sos.h
	./redalert/win32lib/soscomp.h
	./redalert/win32lib/sosdata.h
	./redalert/win32lib/sosdefs.h
	./redalert/win32lib/sosfnct.h
	./redalert/win32lib/sosres.h
	./redalert/win32lib/sound.h
	./redalert/win32lib/soundint.h
	./redalert/win32lib/stamp.inc
	./redalert/win32lib/structs.h
	./redalert/win32lib/svgaprim.inc
	./redalert/win32lib/tile.h
	./redalert/win32lib/timer.cpp
	./redalert/win32lib/timer.h
	./redalert/win32lib/timerdwn.cpp
	./redalert/win32lib/timerini.cpp
	./redalert/win32lib/video.h
	./redalert/win32lib/wincomm.h
	./redalert/win32lib/windows.cpp
	./redalert/win32lib/winhide.cpp
	./redalert/win32lib/writepcx.cpp
	./redalert/win32lib/wsa.cpp
	./redalert/win32lib/wsa.h
	./redalert/win32lib/wwfile.h
	./redalert/win32lib/wwlib32.h
	./redalert/win32lib/wwmem.h
	./redalert/win32lib/wwmem.inc
	./redalert/win32lib/wwstd.h
	./redalert/win32lib/ww_win.h
	./redalert/win32lib/_diptabl.cpp
)

#Red Alert files
set(src_redalert
	./redalert/2keyfram.cpp
	./redalert/aadata.cpp
	./redalert/abstract.cpp
	./redalert/abstract.h
	./redalert/adata.cpp
	./redalert/adpcm.cpp
	./redalert/aircraft.cpp
	./redalert/aircraft.h
	./redalert/anim.cpp
	./redalert/console.cpp
	./redalert/anim.h
	./redalert/audio.cpp
	./redalert/audio.h
	./redalert/audiomix.h
	./redalert/audmix.cpp
	./redalert/b64pipe.cpp
	./redalert/b64pipe.h
	./redalert/b64straw.cpp
	./redalert/b64straw.h
	./redalert/bar.cpp
	./redalert/bar.h
	./redalert/base.cpp
	./redalert/base.h
	./redalert/base64.cpp
	./redalert/base64.h
	./redalert/bbdata.cpp
	./redalert/bdataxml.cpp
	./redalert/bdata.cpp
	./redalert/bench.cpp
	./redalert/bench.h
	./redalert/bfiofile.cpp
	./redalert/bfiofile.h
	./redalert/bigcheck.cpp
	./redalert/bigcheck.h
	./redalert/blowfish.cpp
	./redalert/blowfish.h
	./redalert/blowpipe.cpp
	./redalert/blowpipe.h
	./redalert/blwstraw.cpp
	./redalert/blwstraw.h
	./redalert/buff.cpp
	./redalert/buff.h
	./redalert/bufferx.h
	./redalert/building.cpp
	./redalert/building.h
	./redalert/bullet.cpp
	./redalert/bullet.h
	./redalert/cargo.cpp
	./redalert/cargo.h
	./redalert/carry.cpp
	./redalert/carry.h
	./redalert/cbn_.h
	./redalert/ccdde.cpp
	./redalert/ccdde.h
	./redalert/ccfile.cpp
	./redalert/ccfile.h
	./redalert/ccini.cpp
	./redalert/ccini.h
	./redalert/ccmpath.cpp
	./redalert/ccptr.cpp
	./redalert/ccptr.h
	./redalert/ccten.cpp
	./redalert/cdata.cpp
	./redalert/cdfile.cpp
	./redalert/cdfile.h
	./redalert/cell.cpp
	./redalert/cell.h
	./redalert/checkbox.cpp
	./redalert/checkbox.h
	./redalert/cheklist.cpp
	./redalert/cheklist.h
	./redalert/class.cpp
	./redalert/colrlist.cpp
	./redalert/colrlist.h
	./redalert/combat.cpp
	./redalert/combuf.cpp
	./redalert/combuf.h
	./redalert/cominit.cpp
	./redalert/cominit.h
	./redalert/compat.h
	./redalert/comqueue.cpp
	./redalert/comqueue.h
	./redalert/confdlg.cpp
	./redalert/confdlg.h
	./redalert/connect.cpp
	./redalert/connect.h
	./redalert/connmgr.h
	./redalert/conquer.cpp
	./redalert/conquer.h
	./redalert/const.cpp
	./redalert/control.cpp
	./redalert/control.h
	./redalert/coord.cpp	
	./redalert/coorda.h
	./redalert/crate.cpp
	./redalert/crate.h
	./redalert/crc.cpp
	./redalert/crc.h
	./redalert/crcpipe.cpp
	./redalert/crcpipe.h
	./redalert/crcstraw.cpp
	./redalert/crcstraw.h
	./redalert/credits.cpp
	./redalert/credits.h
	./redalert/crew.cpp
	./redalert/crew.h
	./redalert/cstraw.cpp
	./redalert/cstraw.h
	./redalert/dde.cpp
	./redalert/dde.h
	./redalert/debug.cpp
	./redalert/debug.h
	./redalert/defines.h
	./redalert/descdlg.cpp
	./redalert/descdlg.h
	./redalert/dial8.cpp
	./redalert/dial8.h
	./redalert/dialog.cpp
	./redalert/dibapi.h
	./redalert/dibfile.cpp
	./redalert/dibutil.cpp
	./redalert/dibutil.h
	./redalert/display.cpp
	./redalert/display.h
	./redalert/dllinterface.cpp
	./redalert/dllinterface.h
	./redalert/dllinterfaceeditor.cpp
	./redalert/door.cpp
	./redalert/door.h
	./redalert/dpmi.cpp
	./redalert/dpmi.h
	./redalert/drive.cpp
	./redalert/drive.h
	./redalert/drop.cpp
	./redalert/drop.h
	./redalert/dtable.cpp
	./redalert/dynavec.cpp
	./redalert/edit.cpp
	./redalert/edit.h
	./redalert/egos.cpp
	./redalert/egos.h
	./redalert/ending.cpp
	./redalert/ending.h
	./redalert/event.cpp
	./redalert/event.h
	./redalert/expand.cpp
	./redalert/externs.h
	./redalert/face.cpp
	./redalert/face.h
	./redalert/facing.cpp
	./redalert/facing.h
	./redalert/factory.cpp
	./redalert/factory.h
	./redalert/fakesock.h
	./redalert/field.cpp
	./redalert/field.h
	./redalert/filepcx.h
	./redalert/findpath.cpp
	./redalert/fixed.cpp
	./redalert/fixed.h
	./redalert/flasher.cpp
	./redalert/flasher.h
	./redalert/fly.cpp
	./redalert/fly.h
	./redalert/foot.cpp
	./redalert/foot.h
	./redalert/ftimer.h
	./redalert/function.h
	./redalert/fuse.cpp
	./redalert/fuse.h
	./redalert/gadget.cpp
	./redalert/gadget.h
	./redalert/gamedlg.cpp
	./redalert/gamedlg.h
	./redalert/gauge.cpp
	./redalert/gauge.h
	./redalert/getcpu.cpp
	./redalert/globals.cpp
	./redalert/goptions.cpp
	./redalert/goptions.h
	./redalert/gscreen.cpp
	./redalert/gscreen.h
	./redalert/hdata.cpp
	./redalert/heap.cpp
	./redalert/heap.h
	./redalert/help.cpp
	./redalert/help.h
	./redalert/house.cpp
	./redalert/house.h
	./redalert/hsv.cpp
	./redalert/hsv.h
	./redalert/iconlist.cpp
	./redalert/iconlist.h
	./redalert/idata.cpp
	./redalert/infantry.cpp
	./redalert/infantry.h
	./redalert/ini.cpp
	./redalert/ini.h
	./redalert/inibin.cpp
	./redalert/inicode.cpp
	./redalert/init.cpp
	./redalert/inline.h
	./redalert/int.cpp
	./redalert/int.h
	./redalert/internet.cpp
	./redalert/internet.h
	./redalert/interpal.cpp
	./redalert/intro.cpp
	./redalert/intro.h
	./redalert/iomap.cpp
	./redalert/ioobj.cpp
	./redalert/ipx.cpp
	./redalert/ipx.h
	./redalert/ipx95.cpp
	./redalert/ipx95.h
	./redalert/ipxaddr.cpp
	./redalert/ipxaddr.h
	./redalert/ipxconn.cpp
	./redalert/ipxconn.h
	./redalert/ipxgconn.cpp
	./redalert/ipxgconn.h
	./redalert/ipxmgr.cpp
	./redalert/ipxmgr.h
	./redalert/itable.cpp
	./redalert/jshell.cpp
	./redalert/jshell.h
	./redalert/key.cpp
	./redalert/key.h
	./redalert/keyboard.h
	./redalert/language.h
	./redalert/layer.cpp
	./redalert/layer.h
	./redalert/lcw.cpp
	./redalert/lcw.h
	./redalert/lcwpipe.cpp
	./redalert/lcwpipe.h
	./redalert/lcwstraw.cpp
	./redalert/lcwstraw.h
	./redalert/lcwuncmp.cpp
	./redalert/led.h
	./redalert/license.txt
	./redalert/link.cpp
	./redalert/link.h
	./redalert/lint.h
	./redalert/list.cpp
	./redalert/list.h
	./redalert/listnode.h
	./redalert/loaddlg.cpp
	./redalert/loaddlg.h
	./redalert/logic.cpp
	./redalert/logic.h
	./redalert/lzo.h
	./redalert/lzo1x.h
	./redalert/lzo1x_c.cpp
	./redalert/lzo1x_d.cpp
	./redalert/lzoconf.h
	./redalert/lzopipe.cpp
	./redalert/lzopipe.h
	./redalert/lzostraw.cpp
	./redalert/lzostraw.h
	./redalert/lzo_conf.h
	./redalert/lzw.cpp
	./redalert/lzw.h
	./redalert/lzwpipe.cpp
	./redalert/lzwpipe.h
	./redalert/lzwstraw.cpp
	./redalert/lzwstraw.h
	./redalert/map.cpp
	./redalert/map.h
	./redalert/mapeddlg.cpp
	./redalert/mapedit.cpp
	./redalert/mapedit.h
	./redalert/mapedplc.cpp
	./redalert/mapedtm.cpp
	./redalert/mapsel.cpp
	./redalert/mci.cpp
	./redalert/mci.h
	./redalert/mcimovie.cpp
	./redalert/mcimovie.h
	./redalert/memcheck.h
	./redalert/menus.cpp
	./redalert/message.h
	./redalert/miscasm.cpp
	./redalert/mission.cpp
	./redalert/mission.h
	./redalert/mixfile.cpp
	./redalert/mixfile.h
	./redalert/monoc.cpp
	./redalert/monoc.h
	./redalert/mouse.cpp
	./redalert/mouse.h
	./redalert/movie.h
	./redalert/mp.cpp
	./redalert/mp.h
	./redalert/mpgset.cpp
	./redalert/mpgset.h
	./redalert/mplayer.cpp
	./redalert/mpmgrw.cpp
	./redalert/mpmgrw.h
	./redalert/mpu.cpp
	./redalert/mpu.h
	./redalert/msgbox.cpp
	./redalert/msgbox.h
	./redalert/msglist.cpp
	./redalert/msglist.h
	./redalert/netdlg.cpp
	./redalert/nullconn.cpp
	./redalert/nullconn.h
	./redalert/nulldlg.cpp
	./redalert/nullmgr.cpp
	./redalert/nullmgr.h
	./redalert/object.cpp
	./redalert/object.h
	./redalert/ocidl.h
	./redalert/odata.cpp
	./redalert/options.cpp
	./redalert/options.h
	./redalert/overlay.cpp
	./redalert/overlay.h
	./redalert/unitxml.cpp
	./redalert/packet.cpp
	./redalert/packet.h
	./redalert/palettec.cpp
	./redalert/palettec.h
	./redalert/pipe.cpp
	./redalert/pipe.h
	./redalert/pk.cpp
	./redalert/pk.h
	./redalert/pkpipe.cpp
	./redalert/pkpipe.h
	./redalert/pkstraw.cpp
	./redalert/pkstraw.h
	./redalert/power.cpp
	./redalert/power.h
	./redalert/profile.cpp
	./redalert/queue.cpp
	./redalert/queue.h
	./redalert/radar.cpp
	./redalert/radar.h
	./redalert/radio.cpp
	./redalert/radio.h
	./redalert/ramfile.cpp
	./redalert/ramfile.h
	./redalert/rand.cpp
	./redalert/random.cpp
	./redalert/random.h
	./redalert/rawfile.cpp
	./redalert/rawfile.h
	./redalert/rawolapi.cpp
	./redalert/rawolapi.h
	./redalert/readline.cpp
	./redalert/readline.h
	./redalert/rect.cpp
	./redalert/rect.h
	./redalert/region.h
	./redalert/reinf.cpp
	./redalert/rgb.cpp
	./redalert/rgb.h
	./redalert/rndstraw.cpp
	./redalert/rndstraw.h
	./redalert/rng.h
	./redalert/rotbmp.cpp
	./redalert/rotbmp.h
	./redalert/rules.cpp
	./redalert/rules.h
	./redalert/savedlg.h
	./redalert/saveload.cpp
	./redalert/scenario.cpp
	./redalert/scenario.h
	./redalert/score.cpp
	./redalert/score.h
	./redalert/screen.h
	./redalert/scroll.cpp
	./redalert/scroll.h
	./redalert/sdata.cpp
	./redalert/search.h
	./redalert/seditdlg.cpp
	./redalert/seditdlg.h
	./redalert/sendfile.cpp
	./redalert/seqconn.cpp
	./redalert/seqconn.h
	./redalert/session.cpp
	./redalert/session.h
	./redalert/sha.cpp
	./redalert/sha.h
	./redalert/shape.cpp
	./redalert/shapebtn.cpp
	./redalert/shapebtn.h
	./redalert/shapipe.cpp
	./redalert/shapipe.h
	./redalert/shastraw.cpp
	./redalert/shastraw.h
	./redalert/sidebar.cpp
	./redalert/sidebar.h
	./redalert/sidebarglyphx.cpp
	./redalert/sidebarglyphx.h
	./redalert/slider.cpp
	./redalert/slider.h
	./redalert/smudge.cpp
	./redalert/smudge.h
	./redalert/sounddlg.cpp
	./redalert/sounddlg.h
	./redalert/special.cpp
	./redalert/special.h
	./redalert/newblit.cpp
	./redalert/sprite.cpp
	./redalert/stage.h
	./redalert/startup.cpp
	./redalert/statbtn.cpp
	./redalert/statbtn.h
	./redalert/stats.cpp
	./redalert/straw.cpp
	./redalert/straw.h
	./redalert/stub.cpp
	./redalert/style.h
	./redalert/super.cpp
	./redalert/super.h
	./redalert/surface.cpp
	./redalert/surface.h
	./redalert/tab.cpp
	./redalert/tab.h
	./redalert/taction.cpp
	./redalert/taction.h
	./redalert/target.cpp
	./redalert/target.h
	./redalert/tcpip.cpp
	./redalert/tcpip.h
	./redalert/tdata.cpp
	./redalert/team.cpp
	./redalert/team.h
	./redalert/teamtype.cpp
	./redalert/teamtype.h
	./redalert/techno.cpp
	./redalert/techno.h
	./redalert/template.cpp
	./redalert/template.h
	./redalert/tenmgr.cpp
	./redalert/tenmgr.h
	./redalert/terrain.cpp
	./redalert/terrain.h
	./redalert/tevent.cpp
	./redalert/tevent.h
	./redalert/textbtn.cpp
	./redalert/textbtn.h
	./redalert/theme.cpp
	./redalert/theme.h
	./redalert/texcache.cpp
	./redalert/texcache.h
	./redalert/tileset.cpp
	./redalert/toggle.cpp
	./redalert/toggle.h
	./redalert/tooltip.cpp
	./redalert/tooltip.h
	./redalert/tracker.cpp
	./redalert/trigger.cpp
	./redalert/trigger.h
	./redalert/trigtype.cpp
	./redalert/trigtype.h
	./redalert/txtlabel.cpp
	./redalert/txtlabel.h
	./redalert/type.h
	./redalert/udata.cpp
	./redalert/udpaddr.cpp
	./redalert/unit.cpp
	./redalert/unit.h
	./redalert/utracker.cpp
	./redalert/utracker.h
	./redalert/vdata.cpp
	./redalert/vector.cpp
	./redalert/vector.h
	./redalert/version.cpp
	./redalert/version.h
	./redalert/vessel.cpp
	./redalert/vessel.h
	./redalert/visudlg.cpp
	./redalert/visudlg.h
	./redalert/vortex.cpp
	./redalert/vortex.h
	./redalert/vqamovie.cpp
	./redalert/vqamovie.h
	./redalert/w95trace.cpp
	./redalert/w95trace.h
	./redalert/warhead.cpp
	./redalert/warhead.h
	./redalert/watcom.h
	./redalert/weapon.cpp
	./redalert/weapon.h
	./redalert/winstub.cpp
	./redalert/wolapiob.cpp
	./redalert/wolapiob.h
	./redalert/woldebug.h
	./redalert/woledit.cpp
	./redalert/woledit.h
	./redalert/wolstrng.cpp
	./redalert/wolstrng.h
	./redalert/wol_cgam.cpp
	./redalert/wol_chat.cpp
	./redalert/wol_dnld.cpp
	./redalert/wol_gsup.cpp
	./redalert/wol_gsup.h
	./redalert/wol_logn.cpp
	./redalert/wol_main.cpp
	./redalert/wol_opt.cpp
	./redalert/wsnwlink.h
	./redalert/wspipx.cpp
	./redalert/wspipx.h
	./redalert/wsproto.cpp
	./redalert/wsproto.h
	./redalert/wspudp.cpp
	./redalert/wspudp.h
	./redalert/wwalloc.h
	./redalert/wwfile.h
	./redalert/xpipe.cpp
	./redalert/xpipe.h
	./redalert/xstraw.cpp
	./redalert/xstraw.h
	./redalert/_wsproto.cpp
	./redalert/_wsproto.h
	./redalert/mapscript.cpp
	./redalert/txtprnt.cpp
	./redalert/tilesetxml.cpp
 "redalert/sidebareditor.cpp")

set(src_external
	#glew
	./external/gl/glew.c

	#libsmacker
	./external/libsmacker/smacker.c
	./external/libsmacker/smacker.h
	./external/libsmacker/smk_bitstream.c
	./external/libsmacker/smk_bitstream.h
	./external/libsmacker/smk_hufftree.c
	./external/libsmacker/smk_hufftree.h
	./external/libsmacker/smk_malloc.h
	
	#imgui
	./external/imgui/imconfig.h
	./external/imgui/imgui.cpp
	./external/imgui/imgui.h
	./external/imgui/imgui_draw.cpp
	./external/imgui/imgui_widgets.cpp
	./external/imgui/examples/imgui_impl_opengl3.cpp
	./external/imgui/examples/imgui_impl_opengl3.h
	./external/imgui/examples/imgui_impl_sdl.cpp
	./external/imgui/examples/imgui_impl_sdl.h

	#lua
	./external/lua/lapi.c
	./external/lua/lapi.h
	./external/lua/lauxlib.c
	./external/lua/lauxlib.h
	./external/lua/lbaselib.c
	./external/lua/lbitlib.c
	./external/lua/lcode.c
	./external/lua/lcode.h
	./external/lua/lcorolib.c
	./external/lua/lctype.c
	./external/lua/lctype.h
	./external/lua/ldblib.c
	./external/lua/ldebug.c
	./external/lua/ldebug.h
	./external/lua/ldo.c
	./external/lua/ldo.h
	./external/lua/ldump.c
	./external/lua/lfunc.c
	./external/lua/lfunc.h
	./external/lua/lgc.c
	./external/lua/lgc.h
	./external/lua/linit.c
	./external/lua/liolib.c
	./external/lua/llex.c
	./external/lua/llex.h
	./external/lua/llimits.h
	./external/lua/lmathlib.c
	./external/lua/lmem.c
	./external/lua/lmem.h
	./external/lua/loadlib.c
	./external/lua/lobject.c
	./external/lua/lobject.h
	./external/lua/lopcodes.c
	./external/lua/lopcodes.h
	./external/lua/loslib.c
	./external/lua/lparser.c
	./external/lua/lparser.h
	./external/lua/lprefix.h
	./external/lua/lstate.c
	./external/lua/lstate.h
	./external/lua/lstring.c
	./external/lua/lstring.h
	./external/lua/lstrlib.c
	./external/lua/ltable.c
	./external/lua/ltable.h
	./external/lua/ltablib.c
	./external/lua/ltm.c
	./external/lua/ltm.h
	./external/lua/lua.h
	./external/lua/lua.hpp
	./external/lua/luaconf.h
	./external/lua/lualib.h
	./external/lua/lundump.c
	./external/lua/lundump.h
	./external/lua/lutf8lib.c
	./external/lua/lvm.c
	./external/lua/lvm.h
	./external/lua/lzio.c
	./external/lua/lzio.h
	./external/xml/tinyxml2.cpp
	./external/xml/tinyxml2.h
)
 

if (MSVC)
add_definitions(-D RA_AF -DCHEAT_KEYS -DIMGUI_IMPL_OPENGL_LOADER_GLEW -DSCENARIO_EDITOR -DWINDOWS_IGNORE_PACKING_MISMATCH -DTRUE_FALSE_DEFINED -DWIN32 -DWINDOWS -DENGLISH -D_USRDLL -DREDALERT_EXPORTS)
add_compile_options(/permissive+  /Zc:forScope- /Zp1)
endif(MSVC)

if (UNIX)
add_definitions(-D RA_AF -DCHEAT_KEYS -DIMGUI_IMPL_OPENGL_LOADER_GLEW -DSCENARIO_EDITOR -DTRUE_FALSE_DEFINED -DENGLISH -D_USRDLL -DREDALERT_EXPORTS)
endif(UNIX)

add_executable(RedAlert ${src_io} ${src_win32lib} ${src_redalert} ${src_external})

if (MSVC)
set_target_properties(RedAlert PROPERTIES OUTPUT_NAME "RedAlert" LINK_FLAGS "/PDB:\"RedAlert.pdb\" /SUBSYSTEM:WINDOWS" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../" )
target_link_libraries(RedAlert "opengl32.lib" "winmm.lib" "Ws2_32.lib" "${CMAKE_SOURCE_DIR}/external/devil/ilu.lib" "${CMAKE_SOURCE_DIR}/external/devil/DevIL.lib" "${CMAKE_SOURCE_DIR}/external/sdl2/lib/x64/SDL2.lib" "${CMAKE_SOURCE_DIR}/external/openal/out/build/x64-Release/OpenAL32.lib")
endif(MSVC)

if (UNIX)
set_target_properties(RedAlert PROPERTIES OUTPUT_NAME "RedAlert"  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/.." )
endif(UNIX)

target_include_directories(RedAlert PRIVATE ./external/xml/;./external/devil/;./external/;./redalert/win32lib;./external/lua/;./external/imgui;./external/dxsdk/Include;./external/ffmpeg-win32/include;./external/sdl2/include;./win32lib;./external/openal/include)