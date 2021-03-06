Tutorials
=========

The following sections describe executing several DIVA executables that demonstrate the DIVA :doc:`C++ and Python utility library</api>`. |br|
Visit the `repository <https://github.com/Kitware/DIVA>`_ on how to get and build the DIVA code base.

In this tutorial the ``</path/to/DIVA>`` will refer to one of two directories, depending on your DIVA system.

* If you built DIVA from the source code, ``</path/to/DIVA/>`` refers to the install folder in your DIVA build directory.
* If you obtained a prebuilt DIVA archive, ``</path/to/DIVA/>`` refers to the ``/opt/kitware/DIVA`` directory.



All the source code used to make these executables is provided in the `driver directory of the repository <https://github.com/Kitware/DIVA/tree/master/drivers>`_. 

Execute following script to create all of the necessary environment variables needed.:

.. code-block:: bash

  </path/to/DIVA/>$ source setup_DIVA.sh

The following python packages are required:
* numpy
* matplotlib

You will also need to (apt) install the following packages:
$sudo apt-get install python-tk

As always, we would be happy to hear your comments and receive your contributions on any tutorial.

Schema Examples
---------------

In this section we will focus on the API classes used to writing valid :doc:`KPF</kpf>` result files for scoring. |br|
The DIVA API provides simple classes that impelement all the KPF packet types. 
Each class contains data I/O methods and is able to read and write their contents a file. |br|
The intent of the following executables is to demonstrate the code necessary by the performer to convert their results into the common scorable KPF format.

C++
~~~

A simple `C++ executable <https://github.com/Kitware/DIVA/blob/master/drivers/schema_examples/schema_examples.cpp>`_ is provided to generate some KPF objects. 
To run this example, do the following:

.. code-block:: bash

  </path/to/DIVA/>$ ./bin/schema_examples
  # You will get the following output
  - { meta: Example geometry }
  - { meta: 1 tracks; 50 detection }
  - { meta: min / max frame: 0 942 min / max timestamp: 0 471  }
  - { meta: min / max geometry: 0,289 - 1278 719 ( 1279x431+0+289 ) }
  - { geom: { id0: 0, id1: 66, ts0: 0, g0: 104 349 210 385, src: truth, eval_type: tp, occlusion: heavy, poly0: [[ 100, 399 ],[ 200, 398 ],[ 300, 397 ],],  } }
  ...

Python
~~~~~~

A simple `Python executable script <https://github.com/Kitware/DIVA/blob/master/drivers/schema_examples/schema_examples.py>`_ is provided to generate some KPF objects. 
To run this example, do the following:
 
 .. code-block:: bash

  </path/to/DIVA/>$ python ./python/schema_examples.py
  # You will get the following output
  Geometry Content
  - { meta: Example geometry }
  - { meta: 1 tracks; 50 detection }
  - { meta: min / max frame: 0 942 min / max timestamp: 0 471  }
  - { meta: min / max geometry: 0,289 - 1278 719 ( 1279x431+0+289 ) }
  - { geom: { id0: 0, id1: 66, ts0: 0, g0: 104 349 210 385, src: truth, eval_type: tp, occlusion: heavy, poly0: [[ 100, 399 ],[ 200, 398 ],[ 300, 397 ],],  } }
  ...


Basic Experiment
----------------

In this section we will focus on the API classes needed to :

* Read and write an experiment file using the Experiment API class
* Demonstrate getting individual frames from an experiment source 

C++
~~~

First we will look at how to simply instantiate an instance of the Experiment class, populate it with data, and write its contents to the console. |br|
The intent of this example is to demonstrate the code necessary by the performer to write a valid experiment file. |br|
A simple `C++ executable <https://github.com/Kitware/DIVA/blob/master/drivers/basic_experiment/basic_experiment.cpp>`_ is provided to read and write experiment files. 

To run this example, do the following:

.. code-block:: bash

  # This will write out a new file 'example.yml' experiment file in the current directory
  </path/to/DIVA/>$ ./bin/basic_experiment -s example.yml

As we mentioned above, the DIVA API can provide image frames from the input source specified for an experiment. |br|
Two example experiment files are provided, one that sources a list of images, and another that sources a video file. |br|
The intent of this example is to demonstrate how the performer can use the API to easily get frames from any source and use them in their code. |br|
To run this example, do the following from the BUILD directory :

.. code-block:: bash

  # The image experiment displays frames from a list of images specified in a txt file
  </path/to/DIVA/>$ ./bin/basic_experiment -d ./etc/image_experiment.yml
  # The video experiment displays frames from a video file
  </path/to/DIVA/>$ ./bin/basic_experiment -d ./etc/video_experiment.yml



Darknet Object Detection
------------------------

The intent of this executable is to demonstrate using the DIVA API to:

* Read an experiment file
* Get individual frames from the experiment source
* Perform the Darknet object detection algorithm on each frame
* Translate Darknet results into the KPF Geometry object
* Write the KPF objects into a scorable results file on disk

C++
~~~

A simple `C++ executable <https://github.com/Kitware/DIVA/blob/master/drivers/darknet_detections/darknet_detections.cpp>`_ is provided for this example. 
To run this example, do the following:

.. code-block:: bash

  </path/to/DIVA/>$ ./bin/darknet_detections -r ./etc/image_experiment.yml
  # Note the output 'darknet.geom.yml' file will be written to the algo-out directory under current directory
  # To run Darknet with a video source
  </path/to/DIVA/>$ ./bin/darknet_detections -r ./etc/video_experiment.yml
  # Note the output 'darknet.geom.yml' file will be written to the algo-out directory under current directory
  # Score the out put with this command 
  </path/to/DIVA/>$ python ./python/diva_system.py score ./etc/image_experiment.yml
  # Note the video experiment does not support scoring at this point
  # Scored outputs will be found in the </path/to/DIVA/>/etc/eval-out directory


Activity Detection
------------------

Coming Soon!!

.. |br| raw:: html

   <br />
