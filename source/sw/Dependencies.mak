build_h=$(engine_inc)/build.h $(engine_inc)/compat.h
panel_h=$(sw_src)/panel.h $(sw_src)/mytypes.h $(sw_src)/game.h $(sw_src)/conpic.h
names2_h=$(sw_src)/names2.h $(sw_src)/names.h
game_h=$(sw_src)/game.h $(sw_src)/mytypes.h $(mact_inc)/keyboard.h $(sw_src)/sounds.h $(sw_src)/settings.h $(sw_src)/digi.h $(sw_src)/damage.h $(sw_src)/inv.h $(sw_src)/stag.h
tags_h=$(sw_src)/tags.h $(sw_src)/jtags.h

$(sw_obj)/actor.$o: $(sw_src)/actor.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/weapon.h $(sw_src)/sprite.h $(sw_src)/actor.h $(sw_src)/saveable.h
$(sw_obj)/ai.$o: $(sw_src)/ai.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/ninja.h $(sw_src)/ai.h $(sw_src)/sector.h $(sw_src)/sprite.h $(sw_src)/net.h $(sw_src)/saveable.h
$(sw_obj)/anim.$o: $(sw_src)/anim.cpp $(build_h) $(sw_src)/keys.h $(sw_src)/mytypes.h $(audiolib_inc)/fx_man.h $(audiolib_inc)/music.h $(mact_inc)/scriplib.h $(sw_src)/gamedefs.h $(mact_inc)/keyboard.h $(mact_inc)/control.h $(sw_src)/config.h $(sw_src)/sounds.h $(sw_src)/function.h $(mact_inc)/animlib.h $(sw_src)/anim.h $(game_h) $(sw_src)/colormap.h $(sw_src)/net.h
$(sw_obj)/border.$o: $(sw_src)/border.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(sw_src)/lists.h $(game_h) $(sw_src)/net.h $(sw_src)/text.h
$(sw_obj)/break.$o: $(sw_src)/break.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/break.h $(sw_src)/sprite.h $(sw_src)/sector.h $(sw_src)/light.h $(sw_src)/weapon.h
$(sw_obj)/bunny.$o: $(sw_src)/bunny.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/pal.h $(sw_src)/sprite.h $(sw_src)/actor.h $(sw_src)/weapon.h $(sw_src)/track.h $(sw_src)/saveable.h
$(sw_obj)/cache.$o: $(sw_src)/cache.cpp $(build_h) $(names2_h) $(game_h) $(tags_h) $(sw_src)/break.h $(sw_src)/quake.h $(sw_src)/pal.h $(sw_src)/cache.h $(sw_src)/sounds.h $(sw_src)/net.h
$(sw_obj)/cd.$o: $(sw_src)/cd.cpp $(sw_src)/mytypes.h $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/player.h $(sw_src)/lists.h $(sw_src)/warp.h $(sw_src)/quake.h $(sw_src)/function.h $(mact_inc)/control.h $(sw_src)/trigger.h $(sw_src)/savedef.h $(sw_src)/menus.h $(sw_src)/net.h $(sw_src)/pal.h
$(sw_obj)/cheats.$o: $(sw_src)/cheats.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(sw_src)/mytypes.h $(mact_inc)/control.h $(sw_src)/function.h $(sw_src)/text.h
$(sw_obj)/colormap.$o: $(sw_src)/colormap.cpp $(build_h) $(sw_src)/keys.h $(sw_src)/pal.h $(game_h)
$(sw_obj)/config.$o: $(sw_src)/config.cpp $(build_h) $(sw_src)/settings.h $(sw_src)/mytypes.h $(mact_inc)/scriplib.h $(sw_src)/gamedefs.h $(mact_inc)/keyboard.h $(sw_src)/function.h $(mact_inc)/control.h $(audiolib_inc)/fx_man.h $(sw_src)/sounds.h $(sw_src)/config.h $(sw_src)/common_game.h $(sw_src)/_functio.h $(sw_src)/_config.h
$(sw_obj)/console.$o: $(sw_src)/console.cpp $(build_h) $(sw_src)/mytypes.h $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/player.h $(sw_src)/lists.h $(sw_src)/warp.h $(sw_src)/quake.h $(sw_src)/function.h $(mact_inc)/control.h $(sw_src)/trigger.h $(sw_src)/savedef.h $(sw_src)/menus.h $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/weapon.h $(sw_src)/text.h $(sw_src)/jsector.h
$(sw_obj)/coolg.$o: $(sw_src)/coolg.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/sprite.h $(sw_src)/actor.h $(sw_src)/weapon.h $(sw_src)/track.h $(sw_src)/saveable.h
$(sw_obj)/coolie.$o: $(sw_src)/coolie.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/sprite.h $(sw_src)/actor.h $(sw_src)/track.h $(sw_src)/weapon.h $(sw_src)/saveable.h
$(sw_obj)/copysect.$o: $(sw_src)/copysect.cpp $(build_h) $(names2_h) $(game_h) $(tags_h) $(sw_src)/weapon.h $(sw_src)/sprite.h $(sw_src)/track.h
$(sw_obj)/demo.$o: $(sw_src)/demo.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(sw_src)/net.h $(sw_src)/mytypes.h $(mact_inc)/control.h $(sw_src)/function.h $(sw_src)/demo.h $(sw_src)/player.h $(sw_src)/menus.h
$(sw_obj)/draw.$o: $(sw_src)/draw.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(sw_src)/quake.h $(sw_src)/vis.h $(sw_src)/jsector.h $(sw_src)/reserve.h $(sw_src)/mytypes.h $(mact_inc)/control.h $(sw_src)/function.h $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/player.h $(sw_src)/jtags.h $(sw_src)/parent.h $(sw_src)/cache.h $(sw_src)/text.h $(sw_src)/menus.h $(sw_src)/interp.h $(sw_src)/sector.h
$(sw_obj)/eel.$o: $(sw_src)/eel.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/actor.h $(sw_src)/weapon.h $(sw_src)/track.h $(sw_src)/saveable.h
$(sw_obj)/game.$o: $(sw_src)/game.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/sector.h $(sw_src)/sprite.h $(sw_src)/weapon.h $(sw_src)/player.h $(sw_src)/lists.h $(sw_src)/net.h $(sw_src)/pal.h $(audiolib_inc)/fx_man.h $(sw_src)/mytypes.h $(sw_src)/menus.h $(mact_inc)/control.h $(sw_src)/function.h $(sw_src)/demo.h $(sw_src)/cache.h $(sw_src)/anim.h $(sw_src)/colormap.h $(sw_src)/break.h $(sw_src)/ninja.h $(sw_src)/light.h $(sw_src)/track.h $(sw_src)/jsector.h $(mact_inc)/keyboard.h $(sw_src)/text.h $(audiolib_inc)/music.h $(sw_src)/saveable.h
$(sw_obj)/girlninj.$o: $(sw_src)/girlninj.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/pal.h $(sw_src)/player.h $(sw_src)/net.h $(sw_src)/actor.h $(sw_src)/track.h $(sw_src)/sprite.h $(sw_src)/saveable.h
$(sw_obj)/goro.$o: $(sw_src)/goro.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/actor.h $(sw_src)/track.h $(sw_src)/saveable.h
$(sw_obj)/hornet.$o: $(sw_src)/hornet.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/actor.h $(sw_src)/weapon.h $(sw_src)/track.h $(sw_src)/sprite.h $(sw_src)/saveable.h
$(sw_obj)/interp.$o: $(sw_src)/interp.cpp $(sw_src)/interp.h
$(sw_obj)/interpsh.$o: $(sw_src)/interpsh.cpp $(sw_src)/interp.h
$(sw_obj)/inv.$o: $(sw_src)/inv.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(sw_src)/pal.h $(sw_src)/text.h $(sw_src)/colormap.h $(sw_src)/player.h
$(sw_obj)/jplayer.$o: $(sw_src)/jplayer.cpp $(build_h) $(sw_src)/mytypes.h $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/player.h $(sw_src)/lists.h $(sw_src)/warp.h $(sw_src)/quake.h $(sw_src)/function.h $(mact_inc)/control.h $(sw_src)/trigger.h $(sw_src)/savedef.h $(sw_src)/menus.h $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/bots.h
$(sw_obj)/jsector.$o: $(sw_src)/jsector.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(sw_src)/jnames.h $(panel_h) $(game_h) $(tags_h) $(sw_src)/sector.h $(sw_src)/player.h $(sw_src)/sprite.h $(sw_src)/reserve.h $(sw_src)/jsector.h $(sw_src)/jtags.h $(sw_src)/lists.h $(sw_src)/pal.h $(sw_src)/parent.h
$(sw_obj)/jweapon.$o: $(sw_src)/jweapon.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/break.h $(sw_src)/quake.h $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/ai.h $(sw_src)/weapon.h $(sw_src)/sprite.h $(sw_src)/sector.h $(sw_src)/actor.h $(sw_src)/saveable.h
$(sw_obj)/lava.$o: $(sw_src)/lava.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/actor.h $(sw_src)/track.h
$(sw_obj)/light.$o: $(sw_src)/light.cpp $(build_h) $(names2_h) $(game_h) $(sw_src)/light.h
$(sw_obj)/mclip.$o: $(sw_src)/mclip.cpp $(build_h) $(sw_src)/mytypes.h $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/player.h $(sw_src)/mclip.h
$(sw_obj)/mdastr.$o: $(sw_src)/mdastr.cpp $(build_h) $(sw_src)/mytypes.h $(sw_src)/keys.h $(names2_h) $(game_h)
$(sw_obj)/menus.$o: $(sw_src)/menus.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/sector.h $(sw_src)/sprite.h $(sw_src)/weapon.h $(sw_src)/player.h $(sw_src)/jsector.h $(sw_src)/reserve.h $(mact_inc)/control.h $(sw_src)/menus.h $(sw_src)/sw_strs.h $(sw_src)/pal.h $(sw_src)/function.h $(sw_src)/net.h $(audiolib_inc)/fx_man.h $(audiolib_inc)/music.h $(sw_src)/text.h $(sw_src)/colormap.h
$(sw_obj)/miscactr.$o: $(sw_src)/miscactr.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/quake.h $(audiolib_inc)/fx_man.h $(sw_src)/actor.h $(sw_src)/sector.h $(sw_src)/sprite.h $(sw_src)/saveable.h
$(sw_obj)/morph.$o: $(sw_src)/morph.cpp $(build_h) $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/sector.h $(sw_src)/ai.h $(sw_src)/player.h $(sw_src)/saveable.h
$(sw_obj)/net.$o: $(sw_src)/net.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(tags_h) $(game_h) $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/weapon.h $(sw_src)/text.h $(sw_src)/menus.h
$(sw_obj)/ninja.$o: $(sw_src)/ninja.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/pal.h $(sw_src)/player.h $(sw_src)/net.h $(sw_src)/weapon.h $(sw_src)/track.h $(sw_src)/actor.h $(sw_src)/ninja.h $(sw_src)/sprite.h $(sw_src)/saveable.h
$(sw_obj)/panel.$o: $(sw_src)/panel.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(sw_src)/lists.h $(game_h) $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/vis.h $(sw_src)/weapon.h $(audiolib_inc)/fx_man.h $(sw_src)/text.h $(sw_src)/player.h $(sw_src)/saveable.h
$(sw_obj)/player.$o: $(sw_src)/player.cpp $(build_h) $(sw_src)/mytypes.h $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/player.h $(sw_src)/lists.h $(sw_src)/warp.h $(sw_src)/quake.h $(sw_src)/text.h $(sw_src)/function.h $(mact_inc)/control.h $(sw_src)/trigger.h $(sw_src)/savedef.h $(sw_src)/menus.h $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/demo.h $(sw_src)/mclip.h $(audiolib_inc)/fx_man.h $(sw_src)/sprite.h $(sw_src)/weapon.h $(sw_src)/ninja.h $(sw_src)/break.h $(sw_src)/jsector.h $(sw_src)/sector.h $(sw_src)/actor.h $(sw_src)/colormap.h $(audiolib_inc)/music.h $(sw_src)/vis.h $(sw_src)/track.h $(sw_src)/interp.h $(sw_src)/saveable.h
$(sw_obj)/predict.$o: $(sw_src)/predict.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(sw_src)/net.h
$(sw_obj)/quake.$o: $(sw_src)/quake.cpp $(build_h) $(names2_h) $(game_h) $(tags_h) $(sw_src)/break.h $(sw_src)/quake.h $(sw_src)/sprite.h $(sw_src)/saveable.h
$(sw_obj)/ripper.$o: $(sw_src)/ripper.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/pal.h $(sw_src)/actor.h $(sw_src)/sprite.h $(sw_src)/track.h $(sw_src)/saveable.h
$(sw_obj)/ripper2.$o: $(sw_src)/ripper2.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/pal.h $(sw_src)/actor.h $(sw_src)/sprite.h $(sw_src)/track.h $(audiolib_inc)/fx_man.h $(sw_src)/saveable.h
$(sw_obj)/rooms.$o: $(sw_src)/rooms.cpp $(build_h) $(names2_h) $(panel_h) $(game_h) $(sw_src)/warp.h
$(sw_obj)/rotator.$o: $(sw_src)/rotator.cpp $(build_h) $(names2_h) $(panel_h) $(game_h) $(sw_src)/net.h $(tags_h) $(sw_src)/sector.h $(sw_src)/text.h $(sw_src)/interp.h $(sw_src)/sprite.h $(sw_src)/saveable.h
$(sw_obj)/rts.$o: $(sw_src)/rts.cpp $(sw_src)/mytypes.h $(sw_src)/_rts.h $(sw_src)/rts.h $(sw_src)/cache.h
$(sw_obj)/save.$o: $(sw_src)/save.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/lists.h $(sw_src)/interp.h $(sw_src)/net.h $(sw_src)/savedef.h $(sw_src)/jsector.h $(sw_src)/parent.h $(sw_src)/reserve.h $(sw_src)/mfile.h $(sw_src)/weapon.h $(sw_src)/cache.h $(sw_src)/colormap.h $(sw_src)/player.h $(audiolib_inc)/fx_man.h $(audiolib_inc)/music.h $(sw_src)/saveable.h
$(sw_obj)/scrip2.$o: $(sw_src)/scrip2.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(sw_src)/parse.h $(sw_src)/jsector.h $(sw_src)/parent.h
$(sw_obj)/sector.$o: $(sw_src)/sector.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/sector.h $(sw_src)/player.h $(sw_src)/quake.h $(sw_src)/weapon.h $(sw_src)/jtags.h $(sw_src)/net.h $(sw_src)/break.h $(sw_src)/track.h $(sw_src)/sprite.h $(sw_src)/light.h $(sw_src)/text.h
$(sw_obj)/serp.$o: $(sw_src)/serp.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/actor.h $(sw_src)/track.h $(sw_src)/sector.h
$(sw_obj)/setup.$o: $(sw_src)/setup.cpp $(build_h) $(sw_src)/keys.h $(game_h) $(sw_src)/mytypes.h $(audiolib_inc)/fx_man.h $(audiolib_inc)/music.h $(mact_inc)/scriplib.h $(sw_src)/gamedefs.h $(mact_inc)/keyboard.h $(mact_inc)/control.h $(sw_src)/config.h $(sw_src)/sounds.h $(sw_src)/function.h $(sw_src)/rts.h
$(sw_obj)/skel.$o: $(sw_src)/skel.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/actor.h $(sw_src)/track.h
$(sw_obj)/skull.$o: $(sw_src)/skull.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/weapon.h $(sw_src)/actor.h
$(sw_obj)/slidor.$o: $(sw_src)/slidor.cpp $(build_h) $(names2_h) $(panel_h) $(game_h) $(sw_src)/net.h $(tags_h) $(sw_src)/slidor.h $(sw_src)/slidor.h $(sw_src)/sector.h $(sw_src)/interp.h $(sw_src)/text.h $(sw_src)/sprite.h $(sw_src)/saveable.h
$(sw_obj)/sounds.$o: $(sw_src)/sounds.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(sw_src)/mytypes.h $(audiolib_inc)/fx_man.h $(audiolib_inc)/music.h $(sw_src)/gamedefs.h $(sw_src)/config.h $(panel_h) $(game_h) $(sw_src)/sounds.h $(sw_src)/ai.h $(sw_src)/net.h $(sw_src)/cache.h $(sw_src)/text.h $(sw_src)/rts.h $(sw_src)/menus.h
$(sw_obj)/spike.$o: $(sw_src)/spike.cpp $(build_h) $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/sector.h $(sw_src)/sprite.h $(sw_src)/saveable.h
$(sw_obj)/sprite.$o: $(sw_src)/sprite.cpp $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/warp.h $(sw_src)/light.h $(sw_src)/break.h $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/sounds.h $(sw_src)/interp.h $(sw_src)/sprite.h $(sw_src)/weapon.h $(sw_src)/jsector.h $(sw_src)/text.h $(sw_src)/slidor.h $(sw_src)/player.h $(sw_src)/saveable.h
$(sw_obj)/sumo.$o: $(sw_src)/sumo.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/quake.h $(sw_src)/actor.h $(sw_src)/track.h $(sw_src)/weapon.h $(sw_src)/sector.h
$(sw_obj)/swconfig.$o: $(sw_src)/swconfig.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(sw_src)/mytypes.h $(mact_inc)/scriplib.h $(audiolib_inc)/fx_man.h $(sw_src)/gamedefs.h $(sw_src)/common_game.h $(sw_src)/config.h $(sw_src)/function.h $(sw_src)/rts.h
$(sw_obj)/sync.$o: $(sw_src)/sync.cpp $(sw_src)/keys.h $(game_h) $(tags_h) $(names2_h) $(sw_src)/net.h $(sw_src)/menus.h
$(sw_obj)/text.$o: $(sw_src)/text.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(sw_src)/lists.h $(game_h) $(sw_src)/pal.h $(sw_src)/text.h $(sw_src)/net.h
$(sw_obj)/timer.$o: $(sw_src)/timer.cpp $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/sector.h $(sw_src)/sprite.h $(sw_src)/weapon.h $(audiolib_inc)/fx_man.h $(audiolib_inc)/music.h $(sw_src)/audiofx.h
$(sw_obj)/track.$o: $(sw_src)/track.cpp $(build_h) $(names2_h) $(panel_h) $(tags_h) $(sw_src)/sector.h $(sw_src)/ai.h $(sw_src)/player.h $(game_h) $(sw_src)/net.h $(sw_src)/sprite.h $(sw_src)/track.h $(sw_src)/weapon.h $(sw_src)/saveable.h
$(sw_obj)/vator.$o: $(sw_src)/vator.cpp $(build_h) $(names2_h) $(panel_h) $(game_h) $(sw_src)/net.h $(tags_h) $(sw_src)/sector.h $(sw_src)/interp.h $(sw_src)/text.h $(sw_src)/sprite.h $(sw_src)/weapon.h $(sw_src)/saveable.h
$(sw_obj)/vis.$o: $(sw_src)/vis.cpp $(build_h) $(names2_h) $(game_h) $(tags_h) $(sw_src)/break.h $(sw_src)/quake.h $(sw_src)/pal.h $(sw_src)/sprite.h
$(sw_obj)/wallmove.$o: $(sw_src)/wallmove.cpp $(build_h) $(names2_h) $(game_h) $(tags_h) $(sw_src)/weapon.h $(sw_src)/sprite.h
$(sw_obj)/warp.$o: $(sw_src)/warp.cpp $(build_h) $(names2_h) $(panel_h) $(game_h) $(sw_src)/warp.h
$(sw_obj)/weapon.$o: $(sw_src)/weapon.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/break.h $(sw_src)/quake.h $(sw_src)/net.h $(sw_src)/pal.h $(sw_src)/vis.h $(sw_src)/ai.h $(sw_src)/weapon.h $(sw_src)/anim.h $(sw_src)/damage.h $(sw_src)/sector.h $(sw_src)/sprite.h $(sw_src)/actor.h $(sw_src)/track.h $(sw_src)/player.h
$(sw_obj)/zilla.$o: $(sw_src)/zilla.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/quake.h $(sw_src)/actor.h $(sw_src)/track.h $(audiolib_inc)/fx_man.h
$(sw_obj)/zombie.$o: $(sw_src)/zombie.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(panel_h) $(game_h) $(tags_h) $(sw_src)/ai.h $(sw_src)/pal.h $(sw_src)/player.h $(sw_src)/net.h $(sw_src)/sprite.h $(sw_src)/weapon.h $(sw_src)/actor.h $(sw_src)/track.h
$(sw_obj)/saveable.$o: $(sw_src)/saveable.cpp $(engine_inc)/compat.h $(sw_src)/saveable.h

$(sw_obj)/jnstub.$o: $(sw_src)/jnstub.cpp $(build_h) $(sw_src)/stag.h $(sw_src)/keys.h $(names2_h) $(game_h) $(tags_h) $(sw_src)/pal.h $(engine_inc)/pragmas.h $(engine_inc)/editor.h $(engine_inc)/cache1d.h
$(sw_obj)/brooms.$o: $(sw_src)/brooms.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(engine_inc)/editor.h
$(sw_obj)/bldscript.$o: $(sw_src)/bldscript.cpp $(build_h) $(names2_h) $(game_h) $(sw_src)/parse.h $(engine_inc)/editor.h $(engine_inc)/cache1d.h
$(sw_obj)/jbhlp.$o: $(sw_src)/jbhlp.cpp $(build_h) $(sw_src)/keys.h $(names2_h) $(game_h) $(engine_inc)/editor.h $(engine_inc)/cache1d.h

$(sw_obj)/game_icon.$o: $(sw_rsrc)/game_icon.c

$(sw_obj)/grpscan.$o: $(sw_src)/grpscan.cpp $(engine_inc)/compat.h $(engine_inc)/baselayer.h $(engine_inc)/scriptfile.h $(engine_inc)/cache1d.h $(engine_inc)/crc32.h $(sw_src)/grpscan.h
$(sw_obj)/gameres.$o: $(sw_rsrc)/gameres.rc $(sw_src)/startwin.game.h $(sw_rsrc)/game.bmp $(sw_rsrc)/game_icon.ico
$(sw_obj)/buildres.$o: $(sw_rsrc)/buildres.rc $(sw_rsrc)/build.bmp $(sw_rsrc)/game_icon.ico
$(sw_obj)/startwin.game.$o: $(sw_src)/startwin.game.cpp $(build_h) $(engine_inc)/winlayer.h $(sw_src)/grpscan.h
$(sw_obj)/startgtk.game.$o: $(sw_src)/startgtk.game.cpp $(build_h) $(engine_inc)/dynamicgtk.h $(engine_inc)/baselayer.h $(sw_src)/grpscan.h
