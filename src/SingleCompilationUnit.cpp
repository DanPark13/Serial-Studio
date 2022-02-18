//
// Well, this will be a PITA to maintain. But we can implement some interesting
// optimizations by using this approach, for example this build approach can lead to
// faster code, because the compiler can do more aggressive optimizations on all the
// program's code.
//
// See these articles for more information:
//  https://en.wikipedia.org/wiki/Single_Compilation_Unit
//  https://en.wikipedia.org/wiki/Unity_build
//  https://mesonbuild.com/Unity-builds.html
//
// TODOs:
//  - Write a script that generates this file automagically
//

#if UNITY_BUILD

// clang-format off

//----------------------------------------------------------------------------------------
// Third-party library headers
//----------------------------------------------------------------------------------------

#include "qwt/src/qwt.h"
#include "qwt/src/qwt_abstract_scale_draw.h"
#include "qwt/src/qwt_bezier.h"
#include "qwt/src/qwt_clipper.h"
#include "qwt/src/qwt_color_map.h"
#include "qwt/src/qwt_column_symbol.h"
#include "qwt/src/qwt_date.h"
#include "qwt/src/qwt_date_scale_draw.h"
#include "qwt/src/qwt_date_scale_engine.h"
#include "qwt/src/qwt_dyngrid_layout.h"
#include "qwt/src/qwt_global.h"
#include "qwt/src/qwt_graphic.h"
#include "qwt/src/qwt_interval.h"
#include "qwt/src/qwt_interval_symbol.h"
#include "qwt/src/qwt_math.h"
#include "qwt/src/qwt_magnifier.h"
#include "qwt/src/qwt_null_paintdevice.h"
#include "qwt/src/qwt_painter.h"
#include "qwt/src/qwt_painter_command.h"
#include "qwt/src/qwt_panner.h"
#include "qwt/src/qwt_picker.h"
#include "qwt/src/qwt_picker_machine.h"
#include "qwt/src/qwt_pixel_matrix.h"
#include "qwt/src/qwt_point_3d.h"
#include "qwt/src/qwt_point_polar.h"
#include "qwt/src/qwt_round_scale_draw.h"
#include "qwt/src/qwt_scale_div.h"
#include "qwt/src/qwt_scale_draw.h"
#include "qwt/src/qwt_scale_engine.h"
#include "qwt/src/qwt_scale_map.h"
#include "qwt/src/qwt_spline.h"
#include "qwt/src/qwt_spline_basis.h"
#include "qwt/src/qwt_spline_parametrization.h"
#include "qwt/src/qwt_spline_local.h"
#include "qwt/src/qwt_spline_cubic.h"
#include "qwt/src/qwt_spline_pleasing.h"
#include "qwt/src/qwt_spline_polynomial.h"
#include "qwt/src/qwt_symbol.h"
#include "qwt/src/qwt_system_clock.h"
#include "qwt/src/qwt_text_engine.h"
#include "qwt/src/qwt_text_label.h"
#include "qwt/src/qwt_text.h"
#include "qwt/src/qwt_transform.h"
#include "qwt/src/qwt_widget_overlay.h"
#include "qwt/src/qwt_axis.h"
#include "qwt/src/qwt_axis_id.h"
#include "qwt/src/qwt_curve_fitter.h"
#include "qwt/src/qwt_spline_curve_fitter.h"
#include "qwt/src/qwt_weeding_curve_fitter.h"
#include "qwt/src/qwt_event_pattern.h"
#include "qwt/src/qwt_abstract_legend.h"
#include "qwt/src/qwt_legend.h"
#include "qwt/src/qwt_legend_data.h"
#include "qwt/src/qwt_legend_label.h"
#include "qwt/src/qwt_plot.h"
#include "qwt/src/qwt_plot_renderer.h"
#include "qwt/src/qwt_plot_curve.h"
#include "qwt/src/qwt_plot_dict.h"
#include "qwt/src/qwt_plot_directpainter.h"
#include "qwt/src/qwt_plot_graphicitem.h"
#include "qwt/src/qwt_plot_grid.h"
#include "qwt/src/qwt_plot_histogram.h"
#include "qwt/src/qwt_plot_item.h"
#include "qwt/src/qwt_plot_abstract_barchart.h"
#include "qwt/src/qwt_plot_barchart.h"
#include "qwt/src/qwt_plot_multi_barchart.h"
#include "qwt/src/qwt_plot_intervalcurve.h"
#include "qwt/src/qwt_plot_tradingcurve.h"
#include "qwt/src/qwt_plot_layout.h"
#include "qwt/src/qwt_plot_marker.h"
#include "qwt/src/qwt_plot_zoneitem.h"
#include "qwt/src/qwt_plot_textlabel.h"
#include "qwt/src/qwt_plot_rasteritem.h"
#include "qwt/src/qwt_plot_spectrogram.h"
#include "qwt/src/qwt_plot_spectrocurve.h"
#include "qwt/src/qwt_plot_scaleitem.h"
#include "qwt/src/qwt_plot_legenditem.h"
#include "qwt/src/qwt_plot_seriesitem.h"
#include "qwt/src/qwt_plot_shapeitem.h"
#include "qwt/src/qwt_plot_vectorfield.h"
#include "qwt/src/qwt_plot_abstract_canvas.h"
#include "qwt/src/qwt_plot_canvas.h"
#include "qwt/src/qwt_plot_panner.h"
#include "qwt/src/qwt_plot_picker.h"
#include "qwt/src/qwt_plot_zoomer.h"
#include "qwt/src/qwt_plot_magnifier.h"
#include "qwt/src/qwt_plot_rescaler.h"
#include "qwt/src/qwt_point_mapper.h"
#include "qwt/src/qwt_raster_data.h"
#include "qwt/src/qwt_matrix_raster_data.h"
#include "qwt/src/qwt_vectorfield_symbol.h"
#include "qwt/src/qwt_sampling_thread.h"
#include "qwt/src/qwt_samples.h"
#include "qwt/src/qwt_series_data.h"
#include "qwt/src/qwt_series_store.h"
#include "qwt/src/qwt_point_data.h"
#include "qwt/src/qwt_scale_widget.h"
#include "qwt/src/qwt_polar.h"
#include "qwt/src/qwt_polar_canvas.h"
#include "qwt/src/qwt_polar_curve.h"
#include "qwt/src/qwt_polar_fitter.h"
#include "qwt/src/qwt_polar_grid.h"
#include "qwt/src/qwt_polar_itemdict.h"
#include "qwt/src/qwt_polar_item.h"
#include "qwt/src/qwt_polar_layout.h"
#include "qwt/src/qwt_polar_magnifier.h"
#include "qwt/src/qwt_polar_marker.h"
#include "qwt/src/qwt_polar_panner.h"
#include "qwt/src/qwt_polar_picker.h"
#include "qwt/src/qwt_polar_plot.h"
#include "qwt/src/qwt_polar_renderer.h"
#include "qwt/src/qwt_polar_spectrogram.h"
#include "qwt/src/qwt_abstract_slider.h"
#include "qwt/src/qwt_abstract_scale.h"
#include "qwt/src/qwt_arrow_button.h"
#include "qwt/src/qwt_analog_clock.h"
#include "qwt/src/qwt_compass.h"
#include "qwt/src/qwt_compass_rose.h"
#include "qwt/src/qwt_counter.h"
#include "qwt/src/qwt_dial.h"
#include "qwt/src/qwt_dial_needle.h"
#include "qwt/src/qwt_knob.h"
#include "qwt/src/qwt_slider.h"
#include "qwt/src/qwt_thermo.h"
#include "qwt/src/qwt_wheel.h"
#include "qtcsv/include/qtcsv/qtcsv_global.h"
#include "qtcsv/include/qtcsv/writer.h"
#include "qtcsv/include/qtcsv/variantdata.h"
#include "qtcsv/include/qtcsv/stringdata.h"
#include "qtcsv/include/qtcsv/reader.h"
#include "qtcsv/include/qtcsv/abstractdata.h"
#include "qtcsv/sources/filechecker.h"
#include "qtcsv/sources/contentiterator.h"
#include "qtcsv/sources/symbols.h"
#include "qmqtt/src/mqtt/qmqtt.h"
#include "qmqtt/src/mqtt/qmqtt_client.h"
#include "qmqtt/src/mqtt/qmqtt_client_p.h"
#include "qmqtt/src/mqtt/qmqtt_frame.h"
#include "qmqtt/src/mqtt/qmqtt_global.h"
#include "qmqtt/src/mqtt/qmqtt_message.h"
#include "qmqtt/src/mqtt/qmqtt_message_p.h"
#include "qmqtt/src/mqtt/qmqtt_network_p.h"
#include "qmqtt/src/mqtt/qmqtt_networkinterface.h"
#include "qmqtt/src/mqtt/qmqtt_routedmessage.h"
#include "qmqtt/src/mqtt/qmqtt_router.h"
#include "qmqtt/src/mqtt/qmqtt_routesubscription.h"
#include "qmqtt/src/mqtt/qmqtt_socket_p.h"
#include "qmqtt/src/mqtt/qmqtt_socketinterface.h"
#include "qmqtt/src/mqtt/qmqtt_timer_p.h"
#include "qmqtt/src/mqtt/qmqtt_timerinterface.h"
#include "qmqtt/src/mqtt/qmqtt_ssl_socket_p.h"
#include "QRealFourier/fftreal/Array.h"
#include "QRealFourier/fftreal/Array.h"
#include "QRealFourier/fftreal/DynArray.h"
#include "QRealFourier/fftreal/DynArray.h"
#include "QRealFourier/fftreal/FFTReal.h"
#include "QRealFourier/fftreal/FFTReal.h"
#include "QRealFourier/fftreal/FFTRealFixLen.h"
#include "QRealFourier/fftreal/FFTRealFixLen.h"
#include "QRealFourier/fftreal/FFTRealFixLenParam.h"
#include "QRealFourier/fftreal/FFTRealPassDirect.h"
#include "QRealFourier/fftreal/FFTRealPassDirect.h"
#include "QRealFourier/fftreal/FFTRealPassInverse.h"
#include "QRealFourier/fftreal/FFTRealPassInverse.h"
#include "QRealFourier/fftreal/FFTRealSelect.h"
#include "QRealFourier/fftreal/FFTRealSelect.h"
#include "QRealFourier/fftreal/FFTRealUseTrigo.h"
#include "QRealFourier/fftreal/FFTRealUseTrigo.h"
#include "QRealFourier/fftreal/OscSinCos.h"
#include "QRealFourier/fftreal/OscSinCos.h"
#include "QRealFourier/fftreal/def.h"
#include "QRealFourier/headers/qcomplexnumber.h"
#include "QRealFourier/headers/qfouriercalculator.h"
#include "QRealFourier/headers/qfourierfixedcalculator.h"
#include "QRealFourier/headers/qfouriertransformer.h"
#include "QRealFourier/headers/qfouriervariablecalculator.h"
#include "QRealFourier/headers/qwindowfunction.h"
#include "QSimpleUpdater/include/QSimpleUpdater.h"
#include "QSimpleUpdater/src/Updater.h"
#include "QSimpleUpdater/src/Downloader.h"
#include "KDMacTouchBar/src/kdmactouchbar.h"
#include "KDMacTouchBar/src/kdmactouchbar_global.h"
#include "QMapControl/src/curve.h"
#include "QMapControl/src/geometry.h"
#include "QMapControl/src/imagemanager.h"
#include "QMapControl/src/layer.h"
#include "QMapControl/src/layermanager.h"
#include "QMapControl/src/linestring.h"
#include "QMapControl/src/mapadapter.h"
#include "QMapControl/src/mapcontrol.h"
#include "QMapControl/src/mapnetwork.h"
#include "QMapControl/src/point.h"
#include "QMapControl/src/tilemapadapter.h"
#include "QMapControl/src/wmsmapadapter.h"
#include "QMapControl/src/circlepoint.h"
#include "QMapControl/src/imagepoint.h"
#include "QMapControl/src/gps_position.h"
#include "QMapControl/src/osmmapadapter.h"
#include "QMapControl/src/maplayer.h"
#include "QMapControl/src/geometrylayer.h"
#include "QMapControl/src/googlemapadapter.h"
#include "QMapControl/src/openaerialmapadapter.h"
#include "QMapControl/src/fixedimageoverlay.h"
#include "QMapControl/src/emptymapadapter.h"
#include "QMapControl/src/arrowpoint.h"
#include "QMapControl/src/invisiblepoint.h"
#include "QMapControl/src/qmapcontrol_global.h"
#include "QMapControl/src/bingapimapadapter.h"
#include "QMapControl/src/googleapimapadapter.h"

//----------------------------------------------------------------------------------------
// App headers
//----------------------------------------------------------------------------------------

#include "AppInfo.h"
#include "DataTypes.h"
#include "CSV/Export.h"
#include "CSV/Player.h"
#include "IO/Checksum.h"
#include "IO/Console.h"
#include "IO/DataSources/Serial.h"
#include "IO/DataSources/Network.h"
#include "IO/DataSources/Bluetooth.h"
#include "IO/Manager.h"
#include "JSON/Dataset.h"
#include "JSON/Editor.h"
#include "JSON/Frame.h"
#include "JSON/Generator.h"
#include "JSON/Group.h"
#include "MQTT/Client.h"
#include "Misc/MacExtras.h"
#include "Misc/ModuleManager.h"
#include "Misc/ThemeManager.h"
#include "Misc/TimerEvents.h"
#include "Misc/Translator.h"
#include "Misc/Utilities.h"
#include "Plugins/Server.h"
#include "UI/Dashboard.h"
#include "UI/DashboardWidget.h"
#include "UI/DeclarativeWidget.h"
#include "UI/Widgets/Terminal.h"
#include "UI/Widgets/Accelerometer.h"
#include "UI/Widgets/Bar.h"
#include "UI/Widgets/Common/AnalogGauge.h"
#include "UI/Widgets/Common/AttitudeIndicator.h"
#include "UI/Widgets/Common/BaseWidget.h"
#include "UI/Widgets/Common/ElidedLabel.h"
#include "UI/Widgets/Common/KLed.h"
#include "UI/Widgets/Compass.h"
#include "UI/Widgets/DataGroup.h"
#include "UI/Widgets/FFTPlot.h"
#include "UI/Widgets/GPS.h"
#include "UI/Widgets/Gauge.h"
#include "UI/Widgets/Gyroscope.h"
#include "UI/Widgets/LEDPanel.h"
#include "UI/Widgets/MultiPlot.h"
#include "UI/Widgets/Plot.h"

//----------------------------------------------------------------------------------------
// Third-party source files (Qwt)
//----------------------------------------------------------------------------------------

#include "qwt/src/qwt.cpp"
#include "qwt/src/qwt_abstract_scale_draw.cpp"
#include "qwt/src/qwt_bezier.cpp"
#include "qwt/src/qwt_clipper.cpp"
#include "qwt/src/qwt_color_map.cpp"
#include "qwt/src/qwt_column_symbol.cpp"
#include "qwt/src/qwt_date.cpp"
#include "qwt/src/qwt_date_scale_draw.cpp"
#include "qwt/src/qwt_date_scale_engine.cpp"
#include "qwt/src/qwt_dyngrid_layout.cpp"
#include "qwt/src/qwt_event_pattern.cpp"
#include "qwt/src/qwt_graphic.cpp"
#include "qwt/src/qwt_interval.cpp"
#include "qwt/src/qwt_interval_symbol.cpp"
#include "qwt/src/qwt_math.cpp"
#include "qwt/src/qwt_magnifier.cpp"
#include "qwt/src/qwt_null_paintdevice.cpp"
#include "qwt/src/qwt_painter.cpp"
#include "qwt/src/qwt_painter_command.cpp"
#include "qwt/src/qwt_panner.cpp"
#include "qwt/src/qwt_picker.cpp"
#include "qwt/src/qwt_picker_machine.cpp"
#include "qwt/src/qwt_pixel_matrix.cpp"
#include "qwt/src/qwt_point_3d.cpp"
#include "qwt/src/qwt_point_polar.cpp"
#include "qwt/src/qwt_round_scale_draw.cpp"
#include "qwt/src/qwt_scale_div.cpp"
#include "qwt/src/qwt_scale_draw.cpp"
#include "qwt/src/qwt_scale_map.cpp"
#include "qwt/src/qwt_scale_engine.cpp"
#include "qwt/src/qwt_spline.cpp"
#include "qwt/src/qwt_spline_basis.cpp"
#include "qwt/src/qwt_spline_parametrization.cpp"
#include "qwt/src/qwt_spline_local.cpp"
#include "qwt/src/qwt_spline_cubic.cpp"
#include "qwt/src/qwt_spline_pleasing.cpp"
#include "qwt/src/qwt_spline_polynomial.cpp"
#include "qwt/src/qwt_symbol.cpp"
#include "qwt/src/qwt_system_clock.cpp"
#include "qwt/src/qwt_text_engine.cpp"
#include "qwt/src/qwt_text_label.cpp"
#include "qwt/src/qwt_text.cpp"
#include "qwt/src/qwt_transform.cpp"
#include "qwt/src/qwt_widget_overlay.cpp"
#include "qwt/src/qwt_curve_fitter.cpp"
#include "qwt/src/qwt_spline_curve_fitter.cpp"
#include "qwt/src/qwt_weeding_curve_fitter.cpp"
#include "qwt/src/qwt_abstract_legend.cpp"
#include "qwt/src/qwt_legend.cpp"
#include "qwt/src/qwt_legend_data.cpp"
#include "qwt/src/qwt_legend_label.cpp"
#include "qwt/src/qwt_plot.cpp"
#include "qwt/src/qwt_plot_renderer.cpp"
#include "qwt/src/qwt_plot_axis.cpp"
#include "qwt/src/qwt_plot_curve.cpp"
#include "qwt/src/qwt_plot_dict.cpp"
#include "qwt/src/qwt_plot_directpainter.cpp"
#include "qwt/src/qwt_plot_graphicitem.cpp"
#include "qwt/src/qwt_plot_grid.cpp"
#include "qwt/src/qwt_plot_histogram.cpp"
#include "qwt/src/qwt_plot_item.cpp"
#include "qwt/src/qwt_plot_abstract_barchart.cpp"
#include "qwt/src/qwt_plot_barchart.cpp"
#include "qwt/src/qwt_plot_multi_barchart.cpp"
#include "qwt/src/qwt_plot_intervalcurve.cpp"
#include "qwt/src/qwt_plot_zoneitem.cpp"
#include "qwt/src/qwt_plot_tradingcurve.cpp"
#include "qwt/src/qwt_plot_spectrogram.cpp"
#include "qwt/src/qwt_plot_spectrocurve.cpp"
#include "qwt/src/qwt_plot_scaleitem.cpp"
#include "qwt/src/qwt_plot_legenditem.cpp"
#include "qwt/src/qwt_plot_seriesitem.cpp"
#include "qwt/src/qwt_plot_shapeitem.cpp"
#include "qwt/src/qwt_plot_vectorfield.cpp"
#include "qwt/src/qwt_plot_marker.cpp"
#include "qwt/src/qwt_plot_textlabel.cpp"
#include "qwt/src/qwt_plot_layout.cpp"
#include "qwt/src/qwt_plot_abstract_canvas.cpp"
#include "qwt/src/qwt_plot_canvas.cpp"
#include "qwt/src/qwt_plot_panner.cpp"
#include "qwt/src/qwt_plot_rasteritem.cpp"
#include "qwt/src/qwt_plot_picker.cpp"
#include "qwt/src/qwt_plot_zoomer.cpp"
#include "qwt/src/qwt_plot_magnifier.cpp"
#include "qwt/src/qwt_plot_rescaler.cpp"
#include "qwt/src/qwt_point_mapper.cpp"
#include "qwt/src/qwt_raster_data.cpp"
#include "qwt/src/qwt_matrix_raster_data.cpp"
#include "qwt/src/qwt_vectorfield_symbol.cpp"
#include "qwt/src/qwt_sampling_thread.cpp"
#include "qwt/src/qwt_series_data.cpp"
#include "qwt/src/qwt_point_data.cpp"
#include "qwt/src/qwt_scale_widget.cpp"
#include "qwt/src/qwt_polar_canvas.cpp"
#include "qwt/src/qwt_polar_curve.cpp"
#include "qwt/src/qwt_polar_fitter.cpp"
#include "qwt/src/qwt_polar_grid.cpp"
#include "qwt/src/qwt_polar_item.cpp"
#include "qwt/src/qwt_polar_itemdict.cpp"
#include "qwt/src/qwt_polar_layout.cpp"
#include "qwt/src/qwt_polar_magnifier.cpp"
#include "qwt/src/qwt_polar_marker.cpp"
#include "qwt/src/qwt_polar_panner.cpp"
#include "qwt/src/qwt_polar_picker.cpp"
#include "qwt/src/qwt_polar_plot.cpp"
#include "qwt/src/qwt_polar_renderer.cpp"
#include "qwt/src/qwt_polar_spectrogram.cpp"
#include "qwt/src/qwt_abstract_slider.cpp"
#include "qwt/src/qwt_abstract_scale.cpp"
#include "qwt/src/qwt_arrow_button.cpp"
#include "qwt/src/qwt_analog_clock.cpp"
#include "qwt/src/qwt_compass.cpp"
#include "qwt/src/qwt_compass_rose.cpp"
#include "qwt/src/qwt_counter.cpp"
#include "qwt/src/qwt_dial.cpp"
#include "qwt/src/qwt_dial_needle.cpp"
#include "qwt/src/qwt_knob.cpp"
#include "qwt/src/qwt_slider.cpp"
#include "qwt/src/qwt_thermo.cpp"
#include "qwt/src/qwt_wheel.cpp"

//----------------------------------------------------------------------------------------
// Third-party source files (non-Qwt)
//----------------------------------------------------------------------------------------

#include "qtcsv/sources/writer.cpp"
#include "qtcsv/sources/variantdata.cpp"
#include "qtcsv/sources/stringdata.cpp"
#include "qtcsv/sources/reader.cpp"
#include "qtcsv/sources/contentiterator.cpp"
#include "qmqtt/src/mqtt/qmqtt_client.cpp"
#include "qmqtt/src/mqtt/qmqtt_client_p.cpp"
#include "qmqtt/src/mqtt/qmqtt_frame.cpp"
#include "qmqtt/src/mqtt/qmqtt_message.cpp"
#include "qmqtt/src/mqtt/qmqtt_network.cpp"
#include "qmqtt/src/mqtt/qmqtt_router.cpp"
#include "qmqtt/src/mqtt/qmqtt_routesubscription.cpp"
#include "qmqtt/src/mqtt/qmqtt_socket.cpp"
#include "qmqtt/src/mqtt/qmqtt_timer.cpp"
#include "qmqtt/src/mqtt/qmqtt_ssl_socket.cpp"
#include "QRealFourier/sources/qcomplexnumber.cpp"
#include "QRealFourier/sources/qfouriercalculator.cpp"
#include "QRealFourier/sources/qfourierfixedcalculator.cpp"
#include "QRealFourier/sources/qfouriertransformer.cpp"
#include "QRealFourier/sources/qfouriervariablecalculator.cpp"
#include "QRealFourier/sources/qwindowfunction.cpp"
#include "QSimpleUpdater/src/Updater.cpp"
#include "QSimpleUpdater/src/Downloader.cpp"
#include "QSimpleUpdater/src/QSimpleUpdater.cpp"
#include "QMapControl/src/curve.cpp"
#include "QMapControl/src/geometry.cpp"
#include "QMapControl/src/imagemanager.cpp"
#include "QMapControl/src/layer.cpp"
#include "QMapControl/src/layermanager.cpp"
#include "QMapControl/src/linestring.cpp"
#include "QMapControl/src/mapadapter.cpp"
#include "QMapControl/src/mapcontrol.cpp"
#include "QMapControl/src/mapnetwork.cpp"
#include "QMapControl/src/point.cpp"
#include "QMapControl/src/tilemapadapter.cpp"
#include "QMapControl/src/wmsmapadapter.cpp"
#include "QMapControl/src/circlepoint.cpp"
#include "QMapControl/src/imagepoint.cpp"
#include "QMapControl/src/gps_position.cpp"
#include "QMapControl/src/osmmapadapter.cpp"
#include "QMapControl/src/maplayer.cpp"
#include "QMapControl/src/geometrylayer.cpp"
#include "QMapControl/src/googlemapadapter.cpp"
#include "QMapControl/src/openaerialmapadapter.cpp"
#include "QMapControl/src/fixedimageoverlay.cpp"
#include "QMapControl/src/emptymapadapter.cpp"
#include "QMapControl/src/arrowpoint.cpp"
#include "QMapControl/src/invisiblepoint.cpp"
#include "QMapControl/src/bingapimapadapter.cpp"
#include "QMapControl/src/googleapimapadapter.cpp"

//----------------------------------------------------------------------------------------
// App source files
//----------------------------------------------------------------------------------------

#include "CSV/Export.cpp"
#include "CSV/Player.cpp"
#include "IO/Checksum.cpp"
#include "IO/Console.cpp"
#include "IO/DataSources/Serial.cpp"
#include "IO/DataSources/Network.cpp"
#include "IO/DataSources/Bluetooth.cpp"
#include "IO/Manager.cpp"
#include "JSON/Dataset.cpp"
#include "JSON/Editor.cpp"
#include "JSON/Frame.cpp"
#include "JSON/Generator.cpp"
#include "JSON/Group.cpp"
#include "MQTT/Client.cpp"
#include "Misc/MacExtras.cpp"
#include "Misc/ModuleManager.cpp"
#include "Misc/ThemeManager.cpp"
#include "Misc/TimerEvents.cpp"
#include "Misc/Translator.cpp"
#include "Misc/Utilities.cpp"
#include "Plugins/Server.cpp"
#include "UI/Dashboard.cpp"
#include "UI/DashboardWidget.cpp"
#include "UI/DeclarativeWidget.cpp"
#include "UI/Widgets/Terminal.cpp"
#include "UI/Widgets/Accelerometer.cpp"
#include "UI/Widgets/Bar.cpp"
#include "UI/Widgets/Common/AnalogGauge.cpp"
#include "UI/Widgets/Common/AttitudeIndicator.cpp"
#include "UI/Widgets/Common/BaseWidget.cpp"
#include "UI/Widgets/Common/ElidedLabel.cpp"
#include "UI/Widgets/Common/KLed.cpp"
#include "UI/Widgets/Compass.cpp"
#include "UI/Widgets/DataGroup.cpp"
#include "UI/Widgets/FFTPlot.cpp"
#include "UI/Widgets/GPS.cpp"
#include "UI/Widgets/Gauge.cpp"
#include "UI/Widgets/Gyroscope.cpp"
#include "UI/Widgets/LEDPanel.cpp"
#include "UI/Widgets/MultiPlot.cpp"
#include "UI/Widgets/Plot.cpp"
#include "main.cpp"

//----------------------------------------------------------------------------------------
// Meta object compiler code from third-party libraries
//----------------------------------------------------------------------------------------

#include "moc_qmqtt_networkinterface.cpp"
#include "moc_qmqtt_socketinterface.cpp"
#include "moc_qmqtt_timerinterface.cpp"

#include "moc_curve.cpp"
#include "moc_geometry.cpp"
#include "moc_imagemanager.cpp"
#include "moc_layer.cpp"
#include "moc_layermanager.cpp"
#include "moc_linestring.cpp"
#include "moc_mapadapter.cpp"
#include "moc_mapcontrol.cpp"
#include "moc_mapnetwork.cpp"
#include "moc_point.cpp"
#include "moc_tilemapadapter.cpp"
#include "moc_osmmapadapter.cpp"
#include "moc_maplayer.cpp"
#include "moc_geometrylayer.cpp"
#include "moc_googlemapadapter.cpp"
#include "moc_openaerialmapadapter.cpp"
#include "moc_emptymapadapter.cpp"
#include "moc_bingapimapadapter.cpp"
#include "moc_googleapimapadapter.cpp"

#ifdef Q_OS_MAC
#include "moc_kdmactouchbar.cpp"
#endif

// clang-format on

#endif
