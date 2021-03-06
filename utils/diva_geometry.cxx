/*ckwg +29
* Copyright 2017 by Kitware, Inc.
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

#include "diva_geometry.h"
#include "diva_exceptions.h"

#include <yaml-cpp/yaml.h>
#include <arrows/kpf/yaml/kpf_yaml_writer.h>
#include <arrows/kpf/yaml/kpf_yaml_parser.h>
#include <arrows/kpf/yaml/kpf_canonical_io_adapter.h>

namespace KPF = kwiver::vital::kpf;

class diva_geometry_impl
{
  friend class diva_geometry;
  friend struct diva_bbox_adapter;
  friend struct diva_poly_adapter;

  size_t                                 detection_id;
  size_t                                 track_id;
  size_t                                 frame_id;
  double                                 frame_time_s;
  double                                 frame_absolute_time_us;
  double                                 confidence;
  diva_geometry::bounding_box_pixels     bboxp;
  diva_geometry::source                  source;
  diva_geometry::occlusion               occlusion;
  diva_geometry::evaluation              evaluation;
  diva_geometry::keyframe                keyframe;
  std::vector<std::pair<size_t, size_t>> poly;
  std::map<std::string, double>          classification;

  std::stringstream ss;
};

struct diva_bbox_adapter : public KPF::kpf_box_adapter< diva_geometry_impl >
{
  diva_bbox_adapter() :
    kpf_box_adapter< diva_geometry_impl >(
      // reads the canonical box "b" into the user_detection "d"
      [](const KPF::canonical::bbox_t& b, diva_geometry_impl& d) {
    d.bboxp.x1 = b.x1;
    d.bboxp.y1 = b.y1;
    d.bboxp.x2 = b.x2;
    d.bboxp.y2 = b.y2; },

      // converts a user_detection "d" into a canonical box and returns it
      [](const diva_geometry_impl& d) {
      return KPF::canonical::bbox_t(
        d.bboxp.x1, d.bboxp.y1,
        d.bboxp.x2, d.bboxp.y2); })
  {}
};

struct diva_poly_adapter : public KPF::kpf_poly_adapter< diva_geometry_impl >
{
  diva_poly_adapter() :
    kpf_poly_adapter< diva_geometry_impl >(
      // reads the canonical box "b" into the user_detection "d"
      [](const KPF::canonical::poly_t& b, diva_geometry_impl& d) {
    d.poly.clear();
    for (auto p : b.xy)
    {
      d.poly.push_back(std::pair<double, double>(p.first, p.second));
    }},
      // converts a user_detection "d" into a canonical box and returns it
      [](const diva_geometry_impl& d) {
      KPF::canonical::poly_t p;
      // should check that d's vectors are the same length
      for (auto pair : d.poly)
      {
        p.xy.push_back(std::make_pair(pair.first, pair.second));
      }
      return p; })
  {}
};

diva_geometry::diva_geometry()
{
  _pimpl = new diva_geometry_impl();
  this->clear();
}
diva_geometry::~diva_geometry()
{
  delete _pimpl;
}

void diva_geometry::clear()
{
  _pimpl->detection_id = -1;
  _pimpl->track_id = -1;
  _pimpl->frame_id = -1;
  _pimpl->frame_time_s = -1;
  _pimpl->frame_absolute_time_us = -1;
  _pimpl->confidence = -1;
  _pimpl->bboxp.x1 = -1; _pimpl->bboxp.y1 = -1;
  _pimpl->bboxp.x2 = -1; _pimpl->bboxp.y2 = -1;
  _pimpl->poly.clear();
  _pimpl->source = static_cast<diva_geometry::source>(-1);
  _pimpl->occlusion = static_cast<diva_geometry::occlusion>(-1);
  _pimpl->evaluation = static_cast<diva_geometry::evaluation>(-1);
  _pimpl->keyframe = static_cast<diva_geometry::keyframe>(-1);
  _pimpl->classification.clear();
  _pimpl->poly.clear();
}

bool diva_geometry::is_valid() const
{
  return true;
}

bool diva_geometry::has_detection_id() const
{
  return _pimpl->detection_id != -1;
}
size_t diva_geometry::get_detection_id() const
{
  return _pimpl->detection_id;
}
void diva_geometry::set_detection_id(size_t id)
{
  _pimpl->detection_id = id;
}
void diva_geometry::remove_detection_id()
{
  _pimpl->detection_id = -1;
}


bool diva_geometry::has_track_id() const
{
  return _pimpl->track_id != -1;
}
size_t diva_geometry::get_track_id() const
{
  return _pimpl->track_id;
}
void diva_geometry::set_track_id(size_t id)
{
  _pimpl->track_id = id;
}
void diva_geometry::remove_track_id()
{
  _pimpl->track_id = -1;
}

bool diva_geometry::has_frame_id() const
{
  return _pimpl->frame_id != -1;
}
size_t diva_geometry::get_frame_id() const
{
  return _pimpl->frame_id;
}
void diva_geometry::set_frame_id(size_t id)
{
  _pimpl->frame_id = id;
}
void diva_geometry::remove_frame_id()
{
  _pimpl->frame_id = -1;
}

bool diva_geometry::has_frame_time() const
{
  return _pimpl->frame_time_s >= 0;
}
double diva_geometry::get_frame_time() const
{
  return _pimpl->frame_time_s;
}
void diva_geometry::set_frame_time(double time_s)
{
  _pimpl->frame_time_s = time_s;
}
void diva_geometry::remove_frame_time()
{
  _pimpl->frame_time_s = -1;
}

bool diva_geometry::has_frame_absolute_time() const
{
  return _pimpl->frame_absolute_time_us >= 0;
}
double diva_geometry::get_frame_absolute_time() const
{
  return _pimpl->frame_absolute_time_us;
}
void diva_geometry::set_frame_absolute_time(double time_us)
{
  _pimpl->frame_absolute_time_us = time_us;
}
void diva_geometry::remove_frame_absolute_time()
{
  _pimpl->frame_absolute_time_us = -1;
}

bool diva_geometry::has_confidence() const
{
  return _pimpl->confidence >= 0;
}
double diva_geometry::get_confidence() const
{
  return _pimpl->confidence;
}
void diva_geometry::set_confidence(double conf)
{
  _pimpl->confidence = conf;
}
void diva_geometry::remove_confidence()
{
  _pimpl->confidence = -1;
}

bool diva_geometry::has_source() const
{
  return _pimpl->source != static_cast<diva_geometry::source >(-1);
}
diva_geometry::source diva_geometry::get_source() const
{
  return _pimpl->source;
}
void diva_geometry::set_source(diva_geometry::source s)
{
  _pimpl->source = s;
}
void diva_geometry::remove_source()
{
  _pimpl->source = static_cast<diva_geometry::source>(-1);
}

bool diva_geometry::has_evaluation() const
{
  return _pimpl->evaluation != static_cast<diva_geometry::evaluation >(-1);
}
diva_geometry::evaluation diva_geometry::get_evaluation() const
{
  return _pimpl->evaluation;
}
void diva_geometry::set_evaluation(diva_geometry::evaluation o)
{
  _pimpl->evaluation = o;
}
void diva_geometry::remove_evaluation()
{
  _pimpl->evaluation = static_cast<diva_geometry::evaluation>(-1);
}

bool diva_geometry::has_occlusion() const
{
  return _pimpl->occlusion != static_cast<diva_geometry::occlusion >(-1);
}
diva_geometry::occlusion diva_geometry::get_occlusion() const
{
  return _pimpl->occlusion;
}
void diva_geometry::set_occlusion(diva_geometry::occlusion e)
{
  _pimpl->occlusion = e;
}
void diva_geometry::remove_occlusion()
{
  _pimpl->occlusion = static_cast<diva_geometry::occlusion>(-1);
}

bool diva_geometry::has_keyframe() const
{
  return _pimpl->keyframe != static_cast<diva_geometry::keyframe >(-1);
}
diva_geometry::keyframe diva_geometry::get_keyframe() const
{
  return _pimpl->keyframe;
}
void diva_geometry::set_keyframe(diva_geometry::keyframe kf)
{
  _pimpl->keyframe = kf;
}
void diva_geometry::remove_keyframe()
{
  _pimpl->keyframe = static_cast<diva_geometry::keyframe>(-1);
}

bool diva_geometry::has_bounding_box_pixels() const
{
  if (_pimpl->bboxp.x1 == -1 || _pimpl->bboxp.y1 == -1 ||
    _pimpl->bboxp.x2 == -1 || _pimpl->bboxp.y2 == -1)
    return false;
  return true;
}
const diva_geometry::bounding_box_pixels& diva_geometry::get_bounding_box_pixels() const
{
  return _pimpl->bboxp;
}
void diva_geometry::set_bounding_box_pixels(size_t x1, size_t y1, size_t x2, size_t y2)
{
  _pimpl->bboxp.x1 = x1; _pimpl->bboxp.y1 = y1;
  _pimpl->bboxp.x2 = x2; _pimpl->bboxp.y2 = y2;
}
void diva_geometry::remove_bounding_box_pixels()
{
  _pimpl->bboxp.x1 = -1; _pimpl->bboxp.y1 = -1;
  _pimpl->bboxp.x2 = -1; _pimpl->bboxp.y2 = -1;
}

bool diva_geometry::has_polygon() const
{
  return _pimpl->poly.size() > 0;
}
std::vector<std::pair<size_t, size_t>>& diva_geometry::get_polygon()
{
  return _pimpl->poly;
}
const std::vector<std::pair<size_t, size_t>>& diva_geometry::get_polygon() const
{
  return _pimpl->poly;
}
void diva_geometry::add_polygon_point(const std::pair<size_t, size_t>& pt)
{
  _pimpl->poly.push_back(pt);
}
void diva_geometry::remove_polygon()
{
  _pimpl->poly.clear();
}

bool diva_geometry::has_classification() const
{
  return _pimpl->classification.size() > 0;
}
std::map<std::string, double>& diva_geometry::get_classification()
{
  return _pimpl->classification;
}
const std::map<std::string, double>& diva_geometry::get_classification() const
{
  return _pimpl->classification;
}
void diva_geometry::add_classification(const std::string& name, double probability)
{
  _pimpl->classification.insert(std::pair <std::string, double>(name, probability));
}
std::string diva_geometry::get_max_classification_name() const
{
  if (!has_classification())
    return "";
  double max = -1;
  std::string name;
  for (auto c : _pimpl->classification)
    if (c.second > max)
      name = c.first;
  return name;
}
void diva_geometry::remove_classification()
{
  _pimpl->classification.clear();
}

void diva_geometry::write(std::ostream& os) const
{
  if (!is_valid())
    throw malformed_diva_data_exception("geometry packet is invalid");

  namespace KPFC = KPF::canonical;
  KPF::record_yaml_writer w(os);
  diva_bbox_adapter bba;
  diva_poly_adapter pa;

  const int DETECTOR_DOMAIN = 3; // DIVA objects

  w.set_schema(KPF::schema_style::GEOM);
  w << KPF::writer< KPFC::id_t >(_pimpl->detection_id, KPFC::id_t::DETECTION_ID)
    << KPF::writer< KPFC::id_t >(_pimpl->track_id, KPFC::id_t::TRACK_ID);
  if (_pimpl->frame_id != -1)
    w << KPF::writer< KPFC::timestamp_t >(_pimpl->frame_id, KPFC::timestamp_t::FRAME_NUMBER);
  //else if (_pimpl->frame_time_s != -1)
  //  w << KPF::writer< KPFC::timestamp_t >(_pimpl->frame_time_s, KPFC::timestamp_t::???);
  //else if (_pimpl->frame_absolute_time_us != -1)
  //  w << KPF::writer< KPFC::timestamp_t >(_pimpl->frame_absolute_time_us, KPFC::timestamp_t::???);
  w << KPF::writer< KPFC::bbox_t >(bba(*_pimpl), KPFC::bbox_t::IMAGE_COORDS);
  if (_pimpl->confidence != -1)
    w << KPF::writer< KPFC::conf_t >(_pimpl->confidence, DETECTOR_DOMAIN);
  if (_pimpl->source != static_cast<diva_geometry::source>(-1))
  {
    switch (_pimpl->source)
    {
    case diva_geometry::source::truth:
      w << KPF::writer< KPFC::kv_t >("src", "truth");
      break;
    }
  }
  if (_pimpl->evaluation != static_cast<diva_geometry::evaluation>(-1))
  {
    switch (_pimpl->evaluation)
    {
    case diva_geometry::evaluation::true_positive:
      w << KPF::writer< KPFC::kv_t >("eval_type", "tp");
      break;
    }
  }
  if (_pimpl->occlusion != static_cast<diva_geometry::occlusion>(-1))
  {
    switch (_pimpl->occlusion)
    {
    case diva_geometry::occlusion::partially:
      w << KPF::writer< KPFC::kv_t >("occlusion", "partially");
      break;
    case diva_geometry::occlusion::mostly:
      w << KPF::writer< KPFC::kv_t >("occlusion", "mostly");
      break;
    }
  }
  if (_pimpl->keyframe != static_cast<diva_geometry::keyframe>(-1))
  {
    switch (_pimpl->keyframe)
    {
    case diva_geometry::keyframe::yes:
      w << KPF::writer< KPFC::kv_t >("keyframe", "1");
      break;
    case diva_geometry::keyframe::no:
      w << KPF::writer< KPFC::kv_t >("keyframe", "0");
      break;
    }
  }
  if (!_pimpl->poly.empty())
    w << KPF::writer< KPFC::poly_t>(pa(*_pimpl), KPFC::poly_t::IMAGE_COORDS);
  if (!_pimpl->classification.empty())
  {
    KPFC::cset_t conf_map;
    for (auto i : _pimpl->classification)
      conf_map.d.insert(std::make_pair(i.first, i.second));
    w << KPF::writer< KPFC::cset_t>(conf_map, DETECTOR_DOMAIN);
  }
  w << KPF::record_yaml_writer::endl;
}

std::string diva_geometry::to_string() const
{
  _pimpl->ss.str("");
  write(_pimpl->ss);
  return _pimpl->ss.str();
}

void diva_geometry::from_string(const std::string& p)
{
  clear();

  diva_bbox_adapter bba;
  std::istringstream str(p);
  namespace KPFC = KPF::canonical;
  KPF::kpf_yaml_parser_t parser(str);
  KPF::kpf_reader_t reader(parser);

  reader >> KPF::reader< KPFC::id_t >(_pimpl->detection_id, KPFC::id_t::DETECTION_ID)
    >> KPF::reader< KPFC::id_t >(_pimpl->track_id, KPFC::id_t::TRACK_ID);
  auto frame_id_probe = reader.transfer_packet_from_buffer(
    KPF::packet_header_t(KPF::packet_style::TS, KPFC::timestamp_t::FRAME_NUMBER));
  if (frame_id_probe.first)
    _pimpl->frame_id = frame_id_probe.second.timestamp.d;
  //auto frame_time_probe = reader.transfer_packet_from_buffer(
  //  KPF::packet_header_t(KPF::packet_style::TS, KPFC::timestamp_t::???));
  //if (frame_time_probe.first)
  //  _pimpl->frame_time_s = frame_time_probe.second.???
  //auto frame_absolute_time_probe = reader.transfer_packet_from_buffer(
  //    KPF::packet_header_t(KPF::packet_style::TS, KPFC::timestamp_t::???));
  //if (frame_absolute_time_probe.first)
  //  _pimpl->frame_absolute_time_us = frame_absolute_time_probe.second.???
  auto bbox_probe = reader.transfer_packet_from_buffer(
    KPF::packet_header_t(KPF::packet_style::GEOM, KPFC::bbox_t::IMAGE_COORDS));
  if (bbox_probe.first)
  {
    _pimpl->bboxp.x1 = bbox_probe.second.bbox.x1;
    _pimpl->bboxp.y1 = bbox_probe.second.bbox.y1;
    _pimpl->bboxp.x2 = bbox_probe.second.bbox.x2;
    _pimpl->bboxp.y2 = bbox_probe.second.bbox.y2;
  }
  auto src_probe = reader.transfer_kv_packet_from_buffer("src");
  if (src_probe.first)
  {
    if (src_probe.second.kv.val == "truth")
      _pimpl->source = diva_geometry::source::truth;
  }
  auto occlusion_probe = reader.transfer_kv_packet_from_buffer("occlusion");
  if (occlusion_probe.first)
  {
    if (occlusion_probe.second.kv.val == "partially")
      _pimpl->occlusion = diva_geometry::occlusion::partially;
  }
}