; Auto-generated. Do not edit!


(cl:in-package GCRobotics-msg)


;//! \htmlinclude command_state.msg.html

(cl:defclass <command_state> (roslisp-msg-protocol:ros-message)
  ((state
    :reader state
    :initarg :state
    :type cl:integer
    :initform 0)
   (mask_odom
    :reader mask_odom
    :initarg :mask_odom
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass command_state (<command_state>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <command_state>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'command_state)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name GCRobotics-msg:<command_state> is deprecated: use GCRobotics-msg:command_state instead.")))

(cl:ensure-generic-function 'state-val :lambda-list '(m))
(cl:defmethod state-val ((m <command_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader GCRobotics-msg:state-val is deprecated.  Use GCRobotics-msg:state instead.")
  (state m))

(cl:ensure-generic-function 'mask_odom-val :lambda-list '(m))
(cl:defmethod mask_odom-val ((m <command_state>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader GCRobotics-msg:mask_odom-val is deprecated.  Use GCRobotics-msg:mask_odom instead.")
  (mask_odom m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <command_state>) ostream)
  "Serializes a message object of type '<command_state>"
  (cl:let* ((signed (cl:slot-value msg 'state)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'mask_odom) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <command_state>) istream)
  "Deserializes a message object of type '<command_state>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'state) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:setf (cl:slot-value msg 'mask_odom) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<command_state>)))
  "Returns string type for a message object of type '<command_state>"
  "GCRobotics/command_state")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'command_state)))
  "Returns string type for a message object of type 'command_state"
  "GCRobotics/command_state")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<command_state>)))
  "Returns md5sum for a message object of type '<command_state>"
  "d27b4cb082b2e43166c03ade3dc51539")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'command_state)))
  "Returns md5sum for a message object of type 'command_state"
  "d27b4cb082b2e43166c03ade3dc51539")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<command_state>)))
  "Returns full string definition for message of type '<command_state>"
  (cl:format cl:nil "int32 state~%bool mask_odom~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'command_state)))
  "Returns full string definition for message of type 'command_state"
  (cl:format cl:nil "int32 state~%bool mask_odom~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <command_state>))
  (cl:+ 0
     4
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <command_state>))
  "Converts a ROS message object to a list"
  (cl:list 'command_state
    (cl:cons ':state (state msg))
    (cl:cons ':mask_odom (mask_odom msg))
))
