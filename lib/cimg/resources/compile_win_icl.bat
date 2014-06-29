REM ----------------------------------------------------------------
REM
REM Script to compile CImg examples, using Intel ICL C++ Compiler.
REM
REM ----------------------------------------------------------------

CD ..\examples\
SET CPPFILE=CImg_demo captcha curve_editor2d dtmri_view3d edge_explorer2d fade_images gaussian_fit1d generate_loop_macros hough_transform2d image2ascii image_registration2d image_surface3d jawbreaker mcf_levelsets2d mcf_levelsets3d odykill pde_heatflow2d pde_TschumperleDeriche2d plotter1d radon_transform2d scene3d spherical_function3d tetris tron tutorial wavelet_atrous use_draw_gradient use_nlmeans use_RGBclass use_skeleton

FOR %%F IN (%CPPFILE%) DO (
  icl /I.. /GX /Ox %%F.cpp gdi32.lib user32.lib
)
