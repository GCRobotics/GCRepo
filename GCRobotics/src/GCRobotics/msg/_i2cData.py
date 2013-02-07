"""autogenerated by genpy from GCRobotics/i2cData.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class i2cData(genpy.Message):
  _md5sum = "c17484e7b26b31d2a1c50a7b05afae82"
  _type = "GCRobotics/i2cData"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """uint8 address
uint8 messageType
uint8 messageData
bool expectDataBack

"""
  __slots__ = ['address','messageType','messageData','expectDataBack']
  _slot_types = ['uint8','uint8','uint8','bool']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       address,messageType,messageData,expectDataBack

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(i2cData, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.address is None:
        self.address = 0
      if self.messageType is None:
        self.messageType = 0
      if self.messageData is None:
        self.messageData = 0
      if self.expectDataBack is None:
        self.expectDataBack = False
    else:
      self.address = 0
      self.messageType = 0
      self.messageData = 0
      self.expectDataBack = False

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_struct_4B.pack(_x.address, _x.messageType, _x.messageData, _x.expectDataBack))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 4
      (_x.address, _x.messageType, _x.messageData, _x.expectDataBack,) = _struct_4B.unpack(str[start:end])
      self.expectDataBack = bool(self.expectDataBack)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_struct_4B.pack(_x.address, _x.messageType, _x.messageData, _x.expectDataBack))
    except struct.error as se: self._check_types(se)
    except TypeError as te: self._check_types(te)

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      _x = self
      start = end
      end += 4
      (_x.address, _x.messageType, _x.messageData, _x.expectDataBack,) = _struct_4B.unpack(str[start:end])
      self.expectDataBack = bool(self.expectDataBack)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
_struct_4B = struct.Struct("<4B")
