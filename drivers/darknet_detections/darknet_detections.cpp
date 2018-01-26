/*ckwg +29
* Copyright 2017,2018 by Kitware, Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*  * Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
*  * Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
*  * Neither name of Kitware, Inc. nor the names of any contributors may be used
*    to endorse or promote products derived from this software without specific
*    prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cstdlib>
#include <fstream>
#include <ostream>
#include "diva_geometry.h"
#include "diva_label.h"
#include "diva_experiment.h"
#include "diva_input.h"
#include "diva_exceptions.h"

// KWIVER Spcecific files to run darknet
#include "vital/exceptions.h"
#include "vital/types/image.h"
#include "vital/types/image_container.h"
#include "vital/types/detected_object.h"
#include "vital/types/detected_object_set.h"
#include "vital/algo/image_object_detector.h"
#include "vital/config/config_block.h"
#include "vital/config/config_block_io.h"
#include "vital/plugin_loader/plugin_manager.h"

#include <kwiversys/CommandLineArguments.hxx>
#include <kwiversys/SystemTools.hxx>

//Uncomment this macro to draw detections on each frame and display it
#define DISPLAY_FRAME

#ifdef DISPLAY_FRAME
#include "opencv2/highgui/highgui.hpp"
#include "arrows/ocv/image_container.h"
#include "vital/algo/draw_detected_object_set.h"
#endif

//
// Program options support

typedef kwiversys::CommandLineArguments argT;

struct options_t
{
  bool help;                  // did the user ask for help?
<<<<<<< HEAD
  std::string user_exp_fn;    // location of user's experiment file
  int exit_code;              // if we exit, exit with this code

  explicit options_t( argT& arg );
  bool check_for_sanity_and_help();
};

//
// prototypes
//

=======
  std::string sample_exp_fn;  // location to write sample experiment file
  std::string user_exp_fn;    // location of user's experiment file
  int exit_code;              // if we exit, exit with this code

  explicit options_t( argT& arg );
  bool check_for_sanity_and_help();
};

//
// prototypes
//

bool write_sample_experiment( const std::string& fn );
>>>>>>> refs/remotes/origin/dev/kpf-v4
bool run_experiment( const std::string& fn );

// ----------------------------------------
// main
// ----------------------------------------

int main(int argc, const char* argv[])
{
  //
  // Parse and check the options
  //

  argT arg;
  arg.Initialize( argc, argv );
  options_t options( arg );

  if ( ! arg.Parse() )
  {
    std::cerr << "Problem parsing arguments\n";
    exit( EXIT_FAILURE );
  }

  if ( options.check_for_sanity_and_help() )
  {
    exit( options.exit_code );
  }

  //
<<<<<<< HEAD
  // If requested, run the experiment
=======
  // If requested, write out sample experiment file
>>>>>>> refs/remotes/origin/dev/kpf-v4
  //

<<<<<<< HEAD
  if ( ! options.user_exp_fn.empty() )
  {
    bool okay = run_experiment( options.user_exp_fn );
    const std::string& status = okay? std::string("success") : std::string("error");
    std::cout << status << " running experiment '" << options.user_exp_fn << "'\n";
  }

  //
  // all done
  //
}

// =================================================================

//
// constructor for the options
//

options_t
::options_t( argT& arg):
  help(false), user_exp_fn(""), exit_code( EXIT_SUCCESS )
{
  arg.AddArgument( "-h",       argT::NO_ARGUMENT, &(this->help),
                   "Display usage information" );
  arg.AddArgument( "--help",   argT::NO_ARGUMENT, &(this->help),
                   "Display usage information" );
  arg.AddArgument( "-r",       argT::SPACE_ARGUMENT, &(this->user_exp_fn),
                   "Run the experiment named by the file" );
  arg.AddArgument( "--run",    argT::EQUAL_ARGUMENT, &(this->user_exp_fn),
                   "Run the experiment named by the file" );
}

//
// some sanity checking
//

bool
options_t
::check_for_sanity_and_help()
{
  bool main_should_exit( false ), print_help( this->help );
  if (print_help)
  {
    main_should_exit = true;
    std::cout
      << "This program runs a sample darknet detector using the DIVA framework.\n"
      << "It takes a single argument, which is a DIVA experiment file setting\n"
      << "input and output parameters.\n"
      << "\n"
      << "Options are:\n"
      << "  -h / --help          display this message and exit\n"
      << "  -r FN / --run=FN     run the experiment in file FN\n"
      << "\n";
  }

  return main_should_exit;
}

// =================================================================

bool
run_experiment( const std::string& fn )
{
  namespace KV=kwiver::vital;

  //
  // Framework: initialization
  //
  // The following code loads the experiment, ensures that it's an object_detection
  // task, and opens our output file
  //

  diva_experiment ex;
  if (!ex.read_experiment( fn ))
  {
    throw malformed_diva_data_exception("Invalid experiment configuration");
  }
  if (ex.get_type() != diva_experiment::type::object_detection)
  {
    throw malformed_diva_data_exception("Invalid experiment type; should be object_detection");
  }
  std::ofstream os( ex.get_output_prefix() +".geom.yml" );
=======
  if ( ! options.sample_exp_fn.empty() )
  {
    bool okay = write_sample_experiment( options.sample_exp_fn );
    const std::string& status = okay? std::string("success") : std::string("error");
    std::cout << status << " writing sample experiment file to '" << options.sample_exp_fn << "'\n";
  }

  //
  // If requested, run the experiment
  //

  if ( ! options.user_exp_fn.empty() )
  {
    bool okay = run_experiment( options.user_exp_fn );
    const std::string& status = okay? std::string("success") : std::string("error");
    std::cout << status << " running experiment '" << options.user_exp_fn << "'\n";
  }

  //
  // all done
  //
}

// =================================================================

//
// constructor for the options
//

options_t
::options_t( argT& arg):
  help(false), sample_exp_fn(""), user_exp_fn(""), exit_code( EXIT_SUCCESS )
{
  arg.AddArgument( "-h",       argT::NO_ARGUMENT, &(this->help),
                   "Display usage information" );
  arg.AddArgument( "--help",   argT::NO_ARGUMENT, &(this->help),
                   "Display usage information" );
  arg.AddArgument( "-s",       argT::SPACE_ARGUMENT, &(this->sample_exp_fn),
                   "Write sample experiment file to filename and exit" );
  arg.AddArgument( "--sample", argT::EQUAL_ARGUMENT, &(this->sample_exp_fn),
                   "Write sample experiment file to filename and exit" );
  arg.AddArgument( "-r",       argT::SPACE_ARGUMENT, &(this->user_exp_fn),
                   "Run the experiment named by the file" );
  arg.AddArgument( "--run",    argT::EQUAL_ARGUMENT, &(this->user_exp_fn),
                   "Run the experiment named by the file" );
}

//
// some sanity checking
//

bool
options_t
::check_for_sanity_and_help()
{
  bool main_should_exit( false ), print_help( this->help );
  std::string err_msg("");
  if ( ! this->help )
  {
    if ( (! this->sample_exp_fn.empty()) && (! this->user_exp_fn.empty()))
    {
      err_msg = "Must set only one of '-s' or '-r'";
    }
    else if (this->sample_exp_fn.empty() && this->user_exp_fn.empty())
    {
      err_msg = "Must set at least one of '-s' or '-r'";
    }
    else if ((! this->sample_exp_fn.empty())
             && kwiversys::SystemTools::FileExists( this->sample_exp_fn ))
    {
      err_msg = "Won't write sample over existing file";
    }
    if (! err_msg.empty() )
    {
      print_help = true;
      main_should_exit = true;
      this->exit_code = EXIT_FAILURE;
    }
  }
  if (print_help)
  {
    std::cout
      << "This program runs a sample darknet detector using the DIVA framework.\n"
      << "It takes a single argument, which is a DIVA experiment file setting\n"
      << "input and output parameters.\n"
      << "\n"
      << "Options are:\n"
      << "  -h / --help          display this message and exit\n"
      << "  -r FN / --run=FN     run the experiment in file FN\n"
      << "  -s FN / --sample=FN  write a sample experiment to FN and exit\n"
      << "\n";
  }

  if (! err_msg.empty())
  {
    std::cerr << "Error: " << err_msg << std::endl;
  }

  return main_should_exit;
}

// =================================================================

//
// Create a sample experiment file and write it out to the given
// filename. Return true if successful.
//

bool
write_sample_experiment( const std::string& fn )
{
  diva_experiment ex;

  ex.set_type(diva_experiment::type::object_detection);
  ex.set_dataset_id( "VIRAT_S_000000" );

  // ex.set_input_type(diva_experiment::input_type::video);
  // ex.set_input_source("VIRAT_S_000000.mp4");

  ex.set_input_type(diva_experiment::input_type::file_list);
  ex.set_input_source( "VIRAT_S_000000.frames.txt" );

  ex.set_transport_type(diva_experiment::transport_type::disk);
  ex.set_frame_rate_Hz(30);

  ex.set_input_root_dir( "/your/path/to/input_root_directory" );
  ex.set_output_type(diva_experiment::output_type::file);
  ex.set_output_root_dir( "/your/path/to/output_root_directory" );

  ex.set_algorithm_parameter( "darknet_config_path", "/your/path/to/darknet.config" );

  return ex.write_experiment( fn );
}

// =================================================================

bool
run_experiment( const std::string& fn )
{
  namespace KV=kwiver::vital;

  //
  // Framework: initialization
  //
  // The following code loads the experiment, ensures that it's an object_detection
  // task, and opens our output file
  //

  diva_experiment ex;
  if (!ex.read_experiment( fn ))
  {
    throw malformed_diva_data_exception("Invalid experiment configuration");
  }
  if (ex.get_type() != diva_experiment::type::object_detection)
  {
    throw malformed_diva_data_exception("Invalid experiment type; should be object_detection");
  }
  std::ofstream os( ex.get_output_prefix() +"_geom.kpf" );
>>>>>>> refs/remotes/origin/dev/kpf-v4


  //
  // Detector: initialization
  //
  // The following code initializes KWIVER, instantiates the darknet object
  // detector, and configures it via the config file (named in the experiment file)
  //

  KV::plugin_manager::instance().load_all_plugins();
  KV::algo::image_object_detector_sptr detector =
    KV::algo::image_object_detector::create( "darknet" );
  KV::config_block_sptr config =
    KV::read_config_file( ex.get_algorithm_parameter( "darknet_config_path" ));
  detector->set_configuration( config );

#ifdef DISPLAY_FRAME
  kwiver::vital::algo::draw_detected_object_set_sptr drawer =
    kwiver::vital::algo::draw_detected_object_set::create("ocv");
  drawer->set_configuration(drawer->get_configuration()); // This will default the configuration 
#endif

  //
  // Framework: loading the experiment
  //
  // This code creates a diva input point and intializes it via the
  // experiment file.

  diva_input input;
  if(!input.load_experiment( ex ))
  {
    throw malformed_diva_data_exception( "Failed to initialize experiment input source" );
  }

  //
  // Framework: writing some metadata
  //

  diva_meta meta;
  meta.set_msg( "darknet geometry for dataset "+ex.get_dataset_id() );
  meta.write(os);

  //
  // Framework + detector: setting up buffer variables
  //

  KV::timestamp ts;                // timestamp of the current image
  KV::image_container_sptr frame;  // pointer to the current image
  size_t detection_id = 0;         // Incremented for every detection on every frame

  //
  // Framework + detector:
  // -- looping over input to compute detections
  // -- writing the detections to the framework
  //

  while (input.has_next_frame())
  {
    frame = input.get_next_frame();
    ts = input.get_next_frame_timestamp();

    //
    // Call the detector on the current frame
    //

    kwiver::vital::detected_object_set_sptr detections = detector->detect( frame );

    //
    // Loop over the detections and append them to the kpf geometry file
    //

    auto ie = detections->cend();
    for (auto det = detections->cbegin(); det != ie; ++det)
    {
      diva_geometry geom;              // KPF geometry output buffer

      // set the IDs and timestamps
      geom.set_track_id( detection_id );         // set the ID1 packet
      geom.set_detection_id( detection_id );     // set the ID0 packet
      geom.set_frame_id( ts.get_frame() );       // set the TS0 packet
      geom.set_frame_time( ts.get_time_usec() ); // set the TS1 packet
      ++detection_id;

      // set the classification label->confidence map
      for ( std::string name : (*det)->type()->class_names() )
      {
        double confidence = (*det)->type()->score(name);
        geom.get_classification()[ name ] = confidence;
      }

      // set the detection geometry
      const kwiver::vital::bounding_box_d bbox((*det)->bounding_box());
      geom.set_bounding_box_pixels( bbox.min_x(), bbox.min_y(), bbox.max_x(), bbox.max_y() );

      // write this geometry record
      geom.write(os);

    } // ...for all the detections on this frame

#ifdef DISPLAY_FRAME
    // Draw the detections onto the image and show it via kwiver and the opencv api
    kwiver::vital::image_container_sptr detections_img = drawer->draw(detections, frame);
    // Let's see what it looks like
    cv::Mat _mat = kwiver::arrows::ocv::image_container::vital_to_ocv(detections_img->get_image());
    cv::namedWindow("Darknet Detections", cv::WINDOW_AUTOSIZE);// Create a window for display.
    cv::imshow("Darknet Detections", _mat);                    // Show our image inside it.
    cv::waitKey(2000);                                         // Wait for 2s 
    cvDestroyWindow("Darknet Detections");
#endif

  } // ...for all the frames in the input

  return true;
}


