import Receivers
import re
from pprint import pprint
from lxml import etree as ET

receiver = Receivers.PyReceiver()

name_mapping = {
    'Cube': 'board'
}

_images = {}
_effects = {}
_geometries = {}
_scene_nodes = {}
_material = {}

tree = ET.parse(receiver.getFile())
root = tree.getroot()

NS = "{http://www.collada.org/2005/11/COLLADASchema}"


def _(str):
    return NS + str


def _s(str):
    return str[len(NS):]


class helpers:
    @staticmethod
    def get_float_list(node):
        return [float(datum) for datum in
                re.split(r'\s*', node.text.strip())]

    @staticmethod
    def get_int_list(node):
        return [int(datum) for datum in
                re.split(r'\s*', node.text.strip())]


def parse_images(imgs):
    for img in list(imgs):
        id = img.get('id')
        src = img.find(_('init_from')).text
        _images[id] = src


class effect_parser:
    def __init__(self):
        self.samplers = {}
        self.surfaces = {}
        self.colors     = {}
        self.textures   = []

    def parse_newparams(self, newparams):
        for param in newparams:
            id = param.get('sid')
            for elem in list(param):
                if elem.tag == _("surface"):
                    ref = elem.find(_('init_from')).text
                    self.surfaces[id] = ref
                elif elem.tag == _("sampler2D"):
                    ref = elem.find(_('source')).text
                    self.samplers[id] = ref

    def parse_color(self, node):
        id = node.get('sid')
        self.colors[id] = helpers.get_float_list(node)
        #print self.colors[id]

    def parse_texture(self, node):
        texref = node.get('texture')
        sampler = self.samplers[texref]
        surface = self.surfaces[sampler]
        if surface in _images:
            self.textures.append(_images[surface])

    def parse_phong_node(self, phong):
        for node in phong.iter():
            node_name = _s(node.tag)
            try:
                fun = getattr(self, 'parse_%s' % node_name)
                fun(node)
            except AttributeError, e:
                if e.message.find('instance has no attribute'):
                    pass
                else:
                    raise e

    def parse_effects(self, effect):
        self.effect_id = effect.get('id')
        profile = effect.find(_('profile_COMMON'))
        newparams = profile.findall(_('newparam'))
        self.parse_newparams(newparams)

        technique = profile.find(_('technique'))
        phong = technique.find(_('phong'))
        self.parse_phong_node(phong)


class material_parser:
    def __init__(self, material):
        self.name = material.get('id')
        self.effect_ref = material.find(_('instance_effect')).get('url')[1:]

    def get_effect(self):
        return _effects[self.effect_ref]


class geometry_parser:
    def __init__(self, geometry):
        self.texels = []
        self.name = geometry.get('name')
        mesh = geometry.find(_('mesh'))
        for node in list(mesh):
            tagname = _s(node.tag)
            try:
                fun = getattr(self, 'parse_' + tagname)
                fun(node)
            except AttributeError, e:
                if e.message.find('instance has no attribute'):
                    pass
                else:
                    raise e

    def parse_source(self, node):
        id = node.get('id')
        float_array = node.find(_('float_array'))
        data = helpers.get_float_list(float_array)

        if id.endswith('mesh-positions'):
            self.raw_vertices = data
            return
        elif id.endswith('mesh-normals'):
            self.raw_normals = data
            return
        elif re.search(r'-map-\d', id):
            self.raw_texels = data

    def parse_input_node(self, input_node, indices_all, stride):
        offset   = int(input_node.get('offset'))
        semantic = input_node.get('semantic')

        indices = indices_all[offset::stride]

        if semantic == 'VERTEX':
            vertex_chunks = [self.raw_vertices[i:i + 3] for i in
                             xrange(0, len(self.raw_vertices), 3)]
            self.vertices = []
            for i in indices:
                self.vertices.extend(vertex_chunks[i])
        elif semantic == 'NORMAL':
            normal_chunks = [self.raw_normals[i:i + 3] for i in
                             xrange(0, len(self.raw_normals), 3)]
            self.normals = []
            for i in indices:
                self.normals.extend(normal_chunks[i])
        elif semantic == 'TEXCOORD':
            texel_chunks = [self.raw_texels[i:i + 2] for i in
                            xrange(0, len(self.raw_texels), 2)]
            for i in indices:
                self.texels.extend(texel_chunks[i])

    def parse_polylist(self, node):
        mat = node.get('material')
        if mat:
            self.material = mat

        input_nodes = node.findall(_('input'))
        stride = len(input_nodes)

        indices = helpers.get_int_list(node.find(_('p')))

        for input_node in input_nodes:
            self.parse_input_node(input_node, indices, stride)


class scene_node:
    def __init__(self, node):
        self.name = node.get('name')
        matrix = node.find(_('matrix'))
        if matrix is not None:
            self.parse_matrix(matrix)

    def parse_matrix(self, matrix):
        sid = matrix.get('sid')
        if sid == 'transform':
            self.transform = [float(datum) for datum in
                              re.split(r'\s*', matrix.text.strip())]


class finisher:
    def work_name(self):
        name = geometry.name
        if name not in name_mapping:
            self.recv_name = name
        else:
            self.recv_name = name_mapping[name]

    def send_verts(self):
        receiver.setVertices(
            self.recv_name,
            self.geometry.vertices,
            3
        )

        receiver.setNormals(
            self.recv_name,
            self.geometry.normals,
            3
        )

        receiver.setTexels(
            self.recv_name,
            self.geometry.texels,
            2
        )

    def send_scene_data(self):
        if name in _scene_nodes:
            receiver.setTransformationMatrix(
                self.recv_name,
                _scene_nodes[name].transform
            )

    def send_material(self):
        if self.geometry.material:
            material = _material[self.geometry.material]
            effect = material.get_effect()
            for texture in effect.textures:
                receiver.setTexture(self.recv_name, texture)

            for color in effect.colors:
                receiver.setColor(self.recv_name, color, effect.colors[color])

    def __init__(self, geometry):
        self.geometry = geometry
        self.work_name()

        self.not_wanted = False
        if self.recv_name not in receiver.getReceivers():
            self.not_wanted = True
            return

        self.send_verts()
        self.send_scene_data()
        self.send_material()

##### Parse images
library_images = root.find(_('library_images'))
if library_images is not None:
    parse_images(library_images)

#### Parse effects
library_effects = root.find(_('library_effects'))
if library_effects is not None:
    for effect in list(library_effects):
        ef = effect_parser()
        ef.parse_effects(effect)
        _effects[ef.effect_id] = ef

#### Parse geometries
library_geometries = root.find(_('library_geometries'))
if library_geometries is None:
    raise Exception("No geometries found")

for geometry in list(library_geometries):
    gem = geometry_parser(geometry)
    _geometries[gem.name] = gem

### Parse materials
library_materials = root.find(_('library_materials'))
if library_materials is not None:
    for material in list(library_materials):
        mat = material_parser(material)
        _material[mat.name] = mat

### Parse scenes
library_visual_scenes = root.find(_('library_visual_scenes'))
scene = library_visual_scenes.find(_('visual_scene'))
for node in list(scene):
    node_ = scene_node(node)
    _scene_nodes[node_.name] = node_



for name, geometry in _geometries.iteritems():
    finisher(geometry)
