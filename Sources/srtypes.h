/*
	Copyright:	(c) 2010-2012 Apple Inc. All rights reserved.
*/
#ifndef __glsr_types_h_
#define __glsr_types_h_

#include <stdint.h>

/* AttribMask */
#define srGL_DEPTH_BUFFER_BIT               0x00000100
#define srGL_STENCIL_BUFFER_BIT             0x00000400
#define srGL_COLOR_BUFFER_BIT               0x00004000
#ifdef SRGL_MOCK_GL
	#define GL_DEPTH_BUFFER_BIT               srGL_DEPTH_BUFFER_BIT
	#define GL_STENCIL_BUFFER_BIT             srGL_STENCIL_BUFFER_BIT
	#define GL_COLOR_BUFFER_BIT               srGL_COLOR_BUFFER_BIT
#endif
/* Boolean */
#define srGL_FALSE                          0
#define srGL_TRUE                           1
#ifdef SRGL_MOCK_GL
	#define GL_FALSE                          srGL_FALSE
	#define GL_TRUE                           srGL_TRUE
#endif
/* BeginMode */
#define srGL_POINTS                         0x0000
#define srGL_LINES                          0x0001
#define srGL_LINE_LOOP                      0x0002
#define srGL_LINE_STRIP                     0x0003
#define srGL_TRIANGLES                      0x0004
#define srGL_TRIANGLE_STRIP                 0x0005
#define srGL_TRIANGLE_FAN                   0x0006
#ifdef SRGL_MOCK_GL
	#define GL_POINTS                         srGL_POINTS
	#define GL_LINES                          srGL_LINES
	#define GL_LINE_LOOP                      srGL_LINE_LOOP
	#define GL_LINE_STRIP                     srGL_LINE_STRIP
	#define GL_TRIANGLES                      srGL_TRIANGLES
	#define GL_TRIANGLE_STRIP                 srGL_TRIANGLE_STRIP
	#define GL_TRIANGLE_FAN                   srGL_TRIANGLE_FAN
#endif
/* AlphaFunction */
#define srGL_NEVER                          0x0200
#define srGL_LESS                           0x0201
#define srGL_EQUAL                          0x0202
#define srGL_LEQUAL                         0x0203
#define srGL_GREATER                        0x0204
#define srGL_NOTEQUAL                       0x0205
#define srGL_GEQUAL                         0x0206
#define srGL_ALWAYS                         0x0207
#ifdef SRGL_MOCK_GL
	#define GL_NEVER                          srGL_NEVER
	#define GL_LESS                           srGL_LESS
	#define GL_EQUAL                          srGL_EQUAL
	#define GL_LEQUAL                         srGL_LEQUAL
	#define GL_GREATER                        srGL_GREATER
	#define GL_NOTEQUAL                       srGL_NOTEQUAL
	#define GL_GEQUAL                         srGL_GEQUAL
	#define GL_ALWAYS                         srGL_ALWAYS
#endif
/* BlendingFactorDest */
#define srGL_ZERO                           0
#define srGL_ONE                            1
#define srGL_SRC_COLOR                      0x0300
#define srGL_ONE_MINUS_SRC_COLOR            0x0301
#define srGL_SRC_ALPHA                      0x0302
#define srGL_ONE_MINUS_SRC_ALPHA            0x0303
#define srGL_DST_ALPHA                      0x0304
#define srGL_ONE_MINUS_DST_ALPHA            0x0305
#ifdef SRGL_MOCK_GL
	#define GL_ZERO                           srGL_ZERO
	#define GL_ONE                            srGL_ONE
	#define GL_SRC_COLOR                      srGL_SRC_COLOR
	#define GL_ONE_MINUS_SRC_COLOR            srGL_ONE_MINUS_SRC_COLOR
	#define GL_SRC_ALPHA                      srGL_SRC_ALPHA
	#define GL_ONE_MINUS_SRC_ALPHA            srGL_ONE_MINUS_SRC_ALPHA
	#define GL_DST_ALPHA                      srGL_DST_ALPHA
	#define GL_ONE_MINUS_DST_ALPHA            srGL_ONE_MINUS_DST_ALPHA
#endif
/* BlendingFactorSrc */
#define srGL_DST_COLOR                      0x0306
#define srGL_ONE_MINUS_DST_COLOR            0x0307
#define srGL_SRC_ALPHA_SATURATE             0x0308
#ifdef SRGL_MOCK_GL
	#define GL_DST_COLOR                      srGL_DST_COLOR
	#define GL_ONE_MINUS_DST_COLOR            srGL_ONE_MINUS_DST_COLOR
	#define GL_SRC_ALPHA_SATURATE             srGL_SRC_ALPHA_SATURATE
#endif
/* DrawBufferMode */
#define srGL_NONE                           0
#define srGL_FRONT_LEFT                     0x0400
#define srGL_FRONT_RIGHT                    0x0401
#define srGL_BACK_LEFT                      0x0402
#define srGL_BACK_RIGHT                     0x0403
#define srGL_FRONT                          0x0404
#define srGL_BACK                           0x0405
#define srGL_LEFT                           0x0406
#define srGL_RIGHT                          0x0407
#define srGL_FRONT_AND_BACK                 0x0408
#ifdef SRGL_MOCK_GL
	#define GL_NONE                           srGL_NONE
	#define GL_FRONT_LEFT                     srGL_FRONT_LEFT
	#define GL_FRONT_RIGHT                    srGL_FRONT_RIGHT
	#define GL_BACK_LEFT                      srGL_BACK_LEFT
	#define GL_BACK_RIGHT                     srGL_BACK_RIGHT
	#define GL_FRONT                          srGL_FRONT
	#define GL_BACK                           srGL_BACK
	#define GL_LEFT                           srGL_LEFT
	#define GL_RIGHT                          srGL_RIGHT
	#define GL_FRONT_AND_BACK                 srGL_FRONT_AND_BACK
#endif
/* ErrorCode */
#define srGL_NO_ERROR                       0
#define srGL_INVALID_ENUM                   0x0500
#define srGL_INVALID_VALUE                  0x0501
#define srGL_INVALID_OPERATION              0x0502
#define srGL_OUT_OF_MEMORY                  0x0505
#ifdef SRGL_MOCK_GL
	#define GL_NO_ERROR                       srGL_NO_ERROR
	#define GL_INVALID_ENUM                   srGL_INVALID_ENUM
	#define GL_INVALID_VALUE                  srGL_INVALID_VALUE
	#define GL_INVALID_OPERATION              srGL_INVALID_OPERATION
	#define GL_OUT_OF_MEMORY                  srGL_OUT_OF_MEMORY
#endif

/* FrontFaceDirection */
#define srGL_CW                             0x0900
#define srGL_CCW                            0x0901
#ifdef SRGL_MOCK_GL
	#define GL_CW                             srGL_CW
	#define GL_CCW                            srGL_CCW
#endif
/* GetPName */
#define srGL_POINT_SIZE                     0x0B11
#define srGL_POINT_SIZE_RANGE               0x0B12
#define srGL_POINT_SIZE_GRANULARITY         0x0B13
#define srGL_LINE_SMOOTH                    0x0B20
#define srGL_LINE_WIDTH                     0x0B21
#define srGL_LINE_WIDTH_RANGE               0x0B22
#define srGL_LINE_WIDTH_GRANULARITY         0x0B23
#define srGL_POLYGON_MODE                   0x0B40
#define srGL_POLYGON_SMOOTH                 0x0B41
#define srGL_CULL_FACE                      0x0B44
#define srGL_CULL_FACE_MODE                 0x0B45
#define srGL_FRONT_FACE                     0x0B46
#define srGL_DEPTH_RANGE                    0x0B70
#define srGL_DEPTH_TEST                     0x0B71
#define srGL_DEPTH_WRITEMASK                0x0B72
#define srGL_DEPTH_CLEAR_VALUE              0x0B73
#define srGL_DEPTH_FUNC                     0x0B74
#define srGL_STENCIL_TEST                   0x0B90
#define srGL_STENCIL_CLEAR_VALUE            0x0B91
#define srGL_STENCIL_FUNC                   0x0B92
#define srGL_STENCIL_VALUE_MASK             0x0B93
#define srGL_STENCIL_FAIL                   0x0B94
#define srGL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define srGL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define srGL_STENCIL_REF                    0x0B97
#define srGL_STENCIL_WRITEMASK              0x0B98
#define srGL_VIEWPORT                       0x0BA2
#define srGL_DITHER                         0x0BD0
#define srGL_BLEND_DST                      0x0BE0
#define srGL_BLEND_SRC                      0x0BE1
#define srGL_BLEND                          0x0BE2
#define srGL_LOGIC_OP_MODE                  0x0BF0
#define srGL_COLOR_LOGIC_OP                 0x0BF2
#define srGL_DRAW_BUFFER                    0x0C01
#define srGL_READ_BUFFER                    0x0C02
#define srGL_SCISSOR_BOX                    0x0C10
#define srGL_SCISSOR_TEST                   0x0C11
#define srGL_COLOR_CLEAR_VALUE              0x0C22
#define srGL_COLOR_WRITEMASK                0x0C23
#define srGL_DOUBLEBUFFER                   0x0C32
#define srGL_STEREO                         0x0C33
#define srGL_LINE_SMOOTH_HINT               0x0C52
#define srGL_POLYGON_SMOOTH_HINT            0x0C53
#define srGL_UNPACK_SWAP_BYTES              0x0CF0
#define srGL_UNPACK_LSB_FIRST               0x0CF1
#define srGL_UNPACK_ROW_LENGTH              0x0CF2
#define srGL_UNPACK_SKIP_ROWS               0x0CF3
#define srGL_UNPACK_SKIP_PIXELS             0x0CF4
#define srGL_UNPACK_ALIGNMENT               0x0CF5
#define srGL_PACK_SWAP_BYTES                0x0D00
#define srGL_PACK_LSB_FIRST                 0x0D01
#define srGL_PACK_ROW_LENGTH                0x0D02
#define srGL_PACK_SKIP_ROWS                 0x0D03
#define srGL_PACK_SKIP_PIXELS               0x0D04
#define srGL_PACK_ALIGNMENT                 0x0D05
#define srGL_MAX_TEXTURE_SIZE               0x0D33
#define srGL_MAX_VIEWPORT_DIMS              0x0D3A
#define srGL_SUBPIXEL_BITS                  0x0D50
#define srGL_TEXTURE_1D                     0x0DE0
#define srGL_TEXTURE_2D                     0x0DE1
#define srGL_POLYGON_OFFSET_UNITS           0x2A00
#define srGL_POLYGON_OFFSET_POINT           0x2A01
#define srGL_POLYGON_OFFSET_LINE            0x2A02
#define srGL_POLYGON_OFFSET_FILL            0x8037
#define srGL_POLYGON_OFFSET_FACTOR          0x8038
#define srGL_TEXTURE_BINDING_1D             0x8068
#define srGL_TEXTURE_BINDING_2D             0x8069
#ifdef SRGL_MOCK_GL
	#define GL_POINT_SIZE                     srGL_POINT_SIZE
	#define GL_POINT_SIZE_RANGE               srGL_POINT_SIZE_RANGE
	#define GL_POINT_SIZE_GRANULARITY         srGL_POINT_SIZE_GRANULARITY
	#define GL_LINE_SMOOTH                    srGL_LINE_SMOOTH
	#define GL_LINE_WIDTH                     srGL_LINE_WIDTH
	#define GL_LINE_WIDTH_RANGE               srGL_LINE_WIDTH_RANGE
	#define GL_LINE_WIDTH_GRANULARITY         srGL_LINE_WIDTH_GRANULARITY
	#define GL_POLYGON_MODE                   srGL_POLYGON_MODE
	#define GL_POLYGON_SMOOTH                 srGL_POLYGON_SMOOTH
	#define GL_CULL_FACE                      srGL_CULL_FACE
	#define GL_CULL_FACE_MODE                 srGL_CULL_FACE_MODE
	#define GL_FRONT_FACE                     srGL_FRONT_FACE
	#define GL_DEPTH_RANGE                    srGL_DEPTH_RANGE
	#define GL_DEPTH_TEST                     srGL_DEPTH_TEST
	#define GL_DEPTH_WRITEMASK                srGL_DEPTH_WRITEMASK
	#define GL_DEPTH_CLEAR_VALUE              srGL_DEPTH_CLEAR_VALUE
	#define GL_DEPTH_FUNC                     srGL_DEPTH_FUNC
	#define GL_STENCIL_TEST                   srGL_STENCIL_TEST
	#define GL_STENCIL_CLEAR_VALUE            srGL_STENCIL_CLEAR_VALUE
	#define GL_STENCIL_FUNC                   srGL_STENCIL_FUNC
	#define GL_STENCIL_VALUE_MASK             srGL_STENCIL_VALUE_MASK
	#define GL_STENCIL_FAIL                   srGL_STENCIL_FAIL
	#define GL_STENCIL_PASS_DEPTH_FAIL        srGL_STENCIL_PASS_DEPTH_FAIL
	#define GL_STENCIL_PASS_DEPTH_PASS        srGL_STENCIL_PASS_DEPTH_PASS
	#define GL_STENCIL_REF                    srGL_STENCIL_REF
	#define GL_STENCIL_WRITEMASK              srGL_STENCIL_WRITEMASK
	#define GL_VIEWPORT                       srGL_VIEWPORT
	#define GL_DITHER                         srGL_DITHER
	#define GL_BLEND_DST                      srGL_BLEND_DST
	#define GL_BLEND_SRC                      srGL_BLEND_SRC
	#define GL_BLEND                          srGL_BLEND
	#define GL_LOGIC_OP_MODE                  srGL_LOGIC_OP_MODE
	#define GL_COLOR_LOGIC_OP                 srGL_COLOR_LOGIC_OP
	#define GL_DRAW_BUFFER                    srGL_DRAW_BUFFER
	#define GL_READ_BUFFER                    srGL_READ_BUFFER
	#define GL_SCISSOR_BOX                    srGL_SCISSOR_BOX
	#define GL_SCISSOR_TEST                   srGL_SCISSOR_TEST
	#define GL_COLOR_CLEAR_VALUE              srGL_COLOR_CLEAR_VALUE
	#define GL_COLOR_WRITEMASK                srGL_COLOR_WRITEMASK
	#define GL_DOUBLEBUFFER                   srGL_DOUBLEBUFFER
	#define GL_STEREO                         srGL_STEREO
	#define GL_LINE_SMOOTH_HINT               srGL_LINE_SMOOTH_HINT
	#define GL_POLYGON_SMOOTH_HINT            srGL_POLYGON_SMOOTH_HINT
	#define GL_UNPACK_SWAP_BYTES              srGL_UNPACK_SWAP_BYTES
	#define GL_UNPACK_LSB_FIRST               srGL_UNPACK_LSB_FIRST
	#define GL_UNPACK_ROW_LENGTH              srGL_UNPACK_ROW_LENGTH
	#define GL_UNPACK_SKIP_ROWS               srGL_UNPACK_SKIP_ROWS
	#define GL_UNPACK_SKIP_PIXELS             srGL_UNPACK_SKIP_PIXELS
	#define GL_UNPACK_ALIGNMENT               srGL_UNPACK_ALIGNMENT
	#define GL_PACK_SWAP_BYTES                srGL_PACK_SWAP_BYTES
	#define GL_PACK_LSB_FIRST                 srGL_PACK_LSB_FIRST
	#define GL_PACK_ROW_LENGTH                srGL_PACK_ROW_LENGTH
	#define GL_PACK_SKIP_ROWS                 srGL_PACK_SKIP_ROWS
	#define GL_PACK_SKIP_PIXELS               srGL_PACK_SKIP_PIXELS
	#define GL_PACK_ALIGNMENT                 srGL_PACK_ALIGNMENT
	#define GL_MAX_TEXTURE_SIZE               srGL_MAX_TEXTURE_SIZE
	#define GL_MAX_VIEWPORT_DIMS              srGL_MAX_VIEWPORT_DIMS
	#define GL_SUBPIXEL_BITS                  srGL_SUBPIXEL_BITS
	#define GL_TEXTURE_1D                     srGL_TEXTURE_1D
	#define GL_TEXTURE_2D                     srGL_TEXTURE_2D
	#define GL_POLYGON_OFFSET_UNITS           srGL_POLYGON_OFFSET_UNITS
	#define GL_POLYGON_OFFSET_POINT           srGL_POLYGON_OFFSET_POINT
	#define GL_POLYGON_OFFSET_LINE            srGL_POLYGON_OFFSET_LINE
	#define GL_POLYGON_OFFSET_FILL            srGL_POLYGON_OFFSET_FILL
	#define GL_POLYGON_OFFSET_FACTOR          srGL_POLYGON_OFFSET_FACTOR
	#define GL_TEXTURE_BINDING_1D             srGL_TEXTURE_BINDING_1D
	#define GL_TEXTURE_BINDING_2D             srGL_TEXTURE_BINDING_2D
#endif
/* GetTextureParameter */
#define srGL_TEXTURE_WIDTH                  0x1000
#define srGL_TEXTURE_HEIGHT                 0x1001
#define srGL_TEXTURE_INTERNAL_FORMAT        0x1003
#define srGL_TEXTURE_BORDER_COLOR           0x1004
#define srGL_TEXTURE_RED_SIZE               0x805C
#define srGL_TEXTURE_GREEN_SIZE             0x805D
#define srGL_TEXTURE_BLUE_SIZE              0x805E
#define srGL_TEXTURE_ALPHA_SIZE             0x805F
#ifdef SRGL_MOCK_GL
	#define GL_TEXTURE_WIDTH                  srGL_TEXTURE_WIDTH
	#define GL_TEXTURE_HEIGHT                 srGL_TEXTURE_HEIGHT
	#define GL_TEXTURE_INTERNAL_FORMAT        srGL_TEXTURE_INTERNAL_FORMAT
	#define GL_TEXTURE_BORDER_COLOR           srGL_TEXTURE_BORDER_COLOR
	#define GL_TEXTURE_RED_SIZE               srGL_TEXTURE_RED_SIZE
	#define GL_TEXTURE_GREEN_SIZE             srGL_TEXTURE_GREEN_SIZE
	#define GL_TEXTURE_BLUE_SIZE              srGL_TEXTURE_BLUE_SIZE
	#define GL_TEXTURE_ALPHA_SIZE             srGL_TEXTURE_ALPHA_SIZE
#endif
/* HintMode */
#define srGL_DONT_CARE                      0x1100
#define srGL_FASTEST                        0x1101
#define srGL_NICEST                         0x1102
#ifdef SRGL_MOCK_GL
	#define GL_DONT_CARE                      srGL_DONT_CARE
	#define GL_FASTEST                        srGL_FASTEST
	#define GL_NICEST                         srGL_NICEST
#endif
/* DataType */
#define srGL_BYTE                           0x1400
#define srGL_UNSIGNED_BYTE                  0x1401
#define srGL_SHORT                          0x1402
#define srGL_UNSIGNED_SHORT                 0x1403
#define srGL_INT                            0x1404
#define srGL_UNSIGNED_INT                   0x1405
#define srGL_FLOAT                          0x1406
#define srGL_DOUBLE                         0x140A
#ifdef SRGL_MOCK_GL
	#define GL_BYTE                           srGL_BYTE
	#define GL_UNSIGNED_BYTE                  srGL_UNSIGNED_BYTE
	#define GL_SHORT                          srGL_SHORT
	#define GL_UNSIGNED_SHORT                 srGL_UNSIGNED_SHORT
	#define GL_INT                            srGL_INT
	#define GL_UNSIGNED_INT                   srGL_UNSIGNED_INT
	#define GL_FLOAT                          srGL_FLOAT
	#define GL_DOUBLE                         srGL_DOUBLE
#endif
/* LogicOp */
#define srGL_CLEAR                          0x1500
#define srGL_AND                            0x1501
#define srGL_AND_REVERSE                    0x1502
#define srGL_COPY                           0x1503
#define srGL_AND_INVERTED                   0x1504
#define srGL_NOOP                           0x1505
#define srGL_XOR                            0x1506
#define srGL_OR                             0x1507
#define srGL_NOR                            0x1508
#define srGL_EQUIV                          0x1509
#define srGL_INVERT                         0x150A
#define srGL_OR_REVERSE                     0x150B
#define srGL_COPY_INVERTED                  0x150C
#define srGL_OR_INVERTED                    0x150D
#define srGL_NAND                           0x150E
#define srGL_SET                            0x150F
#ifdef SRGL_MOCK_GL
	#define GL_CLEAR                          srGL_CLEAR
	#define GL_AND                            srGL_AND
	#define GL_AND_REVERSE                    srGL_AND_REVERSE
	#define GL_COPY                           srGL_COPY
	#define GL_AND_INVERTED                   srGL_AND_INVERTED
	#define GL_NOOP                           srGL_NOOP
	#define GL_XOR                            srGL_XOR
	#define GL_OR                             srGL_OR
	#define GL_NOR                            srGL_NOR
	#define GL_EQUIV                          srGL_EQUIV
	#define GL_INVERT                         srGL_INVERT
	#define GL_OR_REVERSE                     srGL_OR_REVERSE
	#define GL_COPY_INVERTED                  srGL_COPY_INVERTED
	#define GL_OR_INVERTED                    srGL_OR_INVERTED
	#define GL_NAND                           srGL_NAND
	#define GL_SET                            srGL_SET
#endif
/* MatrixMode (for gl3.h, FBO attachment type) */
#define srGL_TEXTURE                        0x1702
#ifdef SRGL_MOCK_GL
	#define GL_TEXTURE                        srGL_TEXTURE
#endif
/* PixelCopyType */
#define srGL_COLOR                          0x1800
#define srGL_DEPTH                          0x1801
#define srGL_STENCIL                        0x1802
#ifdef SRGL_MOCK_GL
	#define GL_COLOR                          srGL_COLOR
	#define GL_DEPTH                          srGL_DEPTH
	#define GL_STENCIL                        srGL_STENCIL
#endif
/* PixelFormat */
#define srGL_STENCIL_INDEX                  0x1901
#define srGL_DEPTH_COMPONENT                0x1902
#define srGL_RED                            0x1903
#define srGL_GREEN                          0x1904
#define srGL_BLUE                           0x1905
#define srGL_ALPHA                          0x1906
#define srGL_RGB                            0x1907
#define srGL_RGBA                           0x1908
#ifdef SRGL_MOCK_GL
	#define GL_STENCIL_INDEX                  srGL_STENCIL_INDEX
	#define GL_DEPTH_COMPONENT                srGL_DEPTH_COMPONENT
	#define GL_RED                            srGL_RED
	#define GL_GREEN                          srGL_GREEN
	#define GL_BLUE                           srGL_BLUE
	#define GL_ALPHA                          srGL_ALPHA
	#define GL_RGB                            srGL_RGB
	#define GL_RGBA                           srGL_RGBA
#endif
/* PolygonMode */
#define srGL_POINT                          0x1B00
#define srGL_LINE                           0x1B01
#define srGL_FILL                           0x1B02
#ifdef SRGL_MOCK_GL
	#define GL_POINT                          srGL_POINT
	#define GL_LINE                           srGL_LINE
	#define GL_FILL                           srGL_FILL
#endif
/* StencilOp */
#define srGL_KEEP                           0x1E00
#define srGL_REPLACE                        0x1E01
#define srGL_INCR                           0x1E02
#define srGL_DECR                           0x1E03
#ifdef SRGL_MOCK_GL
	#define GL_KEEP                           srGL_KEEP
	#define GL_REPLACE                        srGL_REPLACE
	#define GL_INCR                           srGL_INCR
	#define GL_DECR                           srGL_DECR
#endif
/* StringName */
#define srGL_VENDOR                         0x1F00
#define srGL_RENDERER                       0x1F01
#define srGL_VERSION                        0x1F02
#define srGL_EXTENSIONS                     0x1F03
#ifdef SRGL_MOCK_GL
	#define GL_VENDOR                         srGL_VENDOR
	#define GL_RENDERER                       srGL_RENDERER
	#define GL_VERSION                        srGL_VERSION
	#define GL_EXTENSIONS                     srGL_EXTENSIONS
#endif
/* TextureMagFilter */
#define srGL_NEAREST                        0x2600
#define srGL_LINEAR                         0x2601
#ifdef SRGL_MOCK_GL
	#define GL_NEAREST                        srGL_NEAREST
	#define GL_LINEAR                         srGL_LINEAR
#endif
/* TextureMinFilter */
#define srGL_NEAREST_MIPMAP_NEAREST         0x2700
#define srGL_LINEAR_MIPMAP_NEAREST          0x2701
#define srGL_NEAREST_MIPMAP_LINEAR          0x2702
#define srGL_LINEAR_MIPMAP_LINEAR           0x2703
#ifdef SRGL_MOCK_GL
	#define GL_NEAREST_MIPMAP_NEAREST         srGL_NEAREST_MIPMAP_NEAREST
	#define GL_LINEAR_MIPMAP_NEAREST          srGL_LINEAR_MIPMAP_NEAREST
	#define GL_NEAREST_MIPMAP_LINEAR          srGL_NEAREST_MIPMAP_LINEAR
	#define GL_LINEAR_MIPMAP_LINEAR           srGL_LINEAR_MIPMAP_LINEAR
#endif
/* TextureParameterName */
#define srGL_TEXTURE_MAG_FILTER             0x2800
#define srGL_TEXTURE_MIN_FILTER             0x2801
#define srGL_TEXTURE_WRAP_S                 0x2802
#define srGL_TEXTURE_WRAP_T                 0x2803
#ifdef SRGL_MOCK_GL
	#define GL_TEXTURE_MAG_FILTER             srGL_TEXTURE_MAG_FILTER
	#define GL_TEXTURE_MIN_FILTER             srGL_TEXTURE_MIN_FILTER
	#define GL_TEXTURE_WRAP_S                 srGL_TEXTURE_WRAP_S
	#define GL_TEXTURE_WRAP_T                 srGL_TEXTURE_WRAP_T
#endif
/* TextureTarget */
#define srGL_PROXY_TEXTURE_1D               0x8063
#define srGL_PROXY_TEXTURE_2D               0x8064
#ifdef SRGL_MOCK_GL
	#define GL_PROXY_TEXTURE_1D               srGL_PROXY_TEXTURE_1D
	#define GL_PROXY_TEXTURE_2D               srGL_PROXY_TEXTURE_2D
#endif
/* TextureWrapMode */
#define srGL_REPEAT                         0x2901
#ifdef SRGL_MOCK_GL
	#define GL_REPEAT                         srGL_REPEAT
#endif
/* PixelInternalFormat */
#define srGL_R3_G3_B2                       0x2A10
#define srGL_RGB4                           0x804F
#define srGL_RGB5                           0x8050
#define srGL_RGB8                           0x8051
#define srGL_RGB10                          0x8052
#define srGL_RGB12                          0x8053
#define srGL_RGB16                          0x8054
#define srGL_RGBA2                          0x8055
#define srGL_RGBA4                          0x8056
#define srGL_RGB5_A1                        0x8057
#define srGL_RGBA8                          0x8058
#define srGL_RGB10_A2                       0x8059
#define srGL_RGBA12                         0x805A
#define srGL_RGBA16                         0x805B
#ifdef SRGL_MOCK_GL
	#define GL_R3_G3_B2                       srGL_R3_G3_B2
	#define GL_RGB4                           srGL_RGB4
	#define GL_RGB5                           srGL_RGB5
	#define GL_RGB8                           srGL_RGB8
	#define GL_RGB10                          srGL_RGB10
	#define GL_RGB12                          srGL_RGB12
	#define GL_RGB16                          srGL_RGB16
	#define GL_RGBA2                          srGL_RGBA2
	#define GL_RGBA4                          srGL_RGBA4
	#define GL_RGB5_A1                        srGL_RGB5_A1
	#define GL_RGBA8                          srGL_RGBA8
	#define GL_RGB10_A2                       srGL_RGB10_A2
	#define GL_RGBA12                         srGL_RGBA12
	#define GL_RGBA16                         srGL_RGBA16
#endif

#define srGL_UNSIGNED_BYTE_3_3_2            0x8032
#define srGL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define srGL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define srGL_UNSIGNED_INT_8_8_8_8           0x8035
#define srGL_UNSIGNED_INT_10_10_10_2        0x8036
#define srGL_TEXTURE_BINDING_3D             0x806A
#define srGL_PACK_SKIP_IMAGES               0x806B
#define srGL_PACK_IMAGE_HEIGHT              0x806C
#define srGL_UNPACK_SKIP_IMAGES             0x806D
#define srGL_UNPACK_IMAGE_HEIGHT            0x806E
#define srGL_TEXTURE_3D                     0x806F
#define srGL_PROXY_TEXTURE_3D               0x8070
#define srGL_TEXTURE_DEPTH                  0x8071
#define srGL_TEXTURE_WRAP_R                 0x8072
#define srGL_MAX_3D_TEXTURE_SIZE            0x8073
#define srGL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define srGL_UNSIGNED_SHORT_5_6_5           0x8363
#define srGL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define srGL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define srGL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define srGL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define srGL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define srGL_BGR                            0x80E0
#define srGL_BGRA                           0x80E1
#define srGL_MAX_ELEMENTS_VERTICES          0x80E8
#define srGL_MAX_ELEMENTS_INDICES           0x80E9
#define srGL_CLAMP_TO_EDGE                  0x812F
#define srGL_TEXTURE_MIN_LOD                0x813A
#define srGL_TEXTURE_MAX_LOD                0x813B
#define srGL_TEXTURE_BASE_LEVEL             0x813C
#define srGL_TEXTURE_MAX_LEVEL              0x813D
#define srGL_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define srGL_SMOOTH_POINT_SIZE_GRANULARITY  0x0B13
#define srGL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define srGL_SMOOTH_LINE_WIDTH_GRANULARITY  0x0B23
#define srGL_ALIASED_LINE_WIDTH_RANGE       0x846E
#ifdef SRGL_MOCK_GL
	#define GL_UNSIGNED_BYTE_3_3_2            srGL_UNSIGNED_BYTE_3_3_2
	#define GL_UNSIGNED_SHORT_4_4_4_4         srGL_UNSIGNED_SHORT_4_4_4_4
	#define GL_UNSIGNED_SHORT_5_5_5_1         srGL_UNSIGNED_SHORT_5_5_5_1
	#define GL_UNSIGNED_INT_8_8_8_8           srGL_UNSIGNED_INT_8_8_8_8
	#define GL_UNSIGNED_INT_10_10_10_2        srGL_UNSIGNED_INT_10_10_10_2
	#define GL_TEXTURE_BINDING_3D             srGL_TEXTURE_BINDING_3D
	#define GL_PACK_SKIP_IMAGES               srGL_PACK_SKIP_IMAGES
	#define GL_PACK_IMAGE_HEIGHT              srGL_PACK_IMAGE_HEIGHT
	#define GL_UNPACK_SKIP_IMAGES             srGL_UNPACK_SKIP_IMAGES
	#define GL_UNPACK_IMAGE_HEIGHT            srGL_UNPACK_IMAGE_HEIGHT
	#define GL_TEXTURE_3D                     srGL_TEXTURE_3D
	#define GL_PROXY_TEXTURE_3D               srGL_PROXY_TEXTURE_3D
	#define GL_TEXTURE_DEPTH                  srGL_TEXTURE_DEPTH
	#define GL_TEXTURE_WRAP_R                 srGL_TEXTURE_WRAP_R
	#define GL_MAX_3D_TEXTURE_SIZE            srGL_MAX_3D_TEXTURE_SIZE
	#define GL_UNSIGNED_BYTE_2_3_3_REV        srGL_UNSIGNED_BYTE_2_3_3_REV
	#define GL_UNSIGNED_SHORT_5_6_5           srGL_UNSIGNED_SHORT_5_6_5
	#define GL_UNSIGNED_SHORT_5_6_5_REV       srGL_UNSIGNED_SHORT_5_6_5_REV
	#define GL_UNSIGNED_SHORT_4_4_4_4_REV     srGL_UNSIGNED_SHORT_4_4_4_4_REV
	#define GL_UNSIGNED_SHORT_1_5_5_5_REV     srGL_UNSIGNED_SHORT_1_5_5_5_REV
	#define GL_UNSIGNED_INT_8_8_8_8_REV       srGL_UNSIGNED_INT_8_8_8_8_REV
	#define GL_UNSIGNED_INT_2_10_10_10_REV    srGL_UNSIGNED_INT_2_10_10_10_REV
	#define GL_BGR                            srGL_BGR
	#define GL_BGRA                           srGL_BGRA
	#define GL_MAX_ELEMENTS_VERTICES          srGL_MAX_ELEMENTS_VERTICES
	#define GL_MAX_ELEMENTS_INDICES		      srGL_MAX_ELEMENTS_INDICES
	#define GL_CLAMP_TO_EDGE                  srGL_CLAMP_TO_EDGE
	#define GL_TEXTURE_MIN_LOD                srGL_TEXTURE_MIN_LOD
	#define GL_TEXTURE_MAX_LOD                srGL_TEXTURE_MAX_LOD
	#define GL_TEXTURE_BASE_LEVEL             srGL_TEXTURE_BASE_LEVEL
	#define GL_TEXTURE_MAX_LEVEL              srGL_TEXTURE_MAX_LEVEL
	#define GL_SMOOTH_POINT_SIZE_RANGE        srGL_SMOOTH_POINT_SIZE_RANGE
	#define GL_SMOOTH_POINT_SIZE_GRANULARITY  srGL_SMOOTH_POINT_SIZE_GRANULARITY
	#define GL_SMOOTH_LINE_WIDTH_RANGE        srGL_SMOOTH_LINE_WIDTH_RANGE
	#define GL_SMOOTH_LINE_WIDTH_GRANULARITY  srGL_SMOOTH_LINE_WIDTH_GRANULARITY
	#define GL_ALIASED_LINE_WIDTH_RANGE	      srGL_ALIASED_LINE_WIDTH_RANGE
#endif

#define srGL_CONSTANT_COLOR                 0x8001
#define srGL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define srGL_CONSTANT_ALPHA                 0x8003
#define srGL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define srGL_BLEND_COLOR                    0x8005
#define srGL_FUNC_ADD                       0x8006
#define srGL_MIN                            0x8007
#define srGL_MAX                            0x8008
#define srGL_BLEND_EQUATION                 0x8009
#define srGL_FUNC_SUBTRACT                  0x800A
#define srGL_FUNC_REVERSE_SUBTRACT          0x800B
#ifdef SRGL_MOCK_GL
	#define GL_CONSTANT_COLOR                 srGL_CONSTANT_COLOR
	#define GL_ONE_MINUS_CONSTANT_COLOR       srGL_ONE_MINUS_CONSTANT_COLOR
	#define GL_CONSTANT_ALPHA                 srGL_CONSTANT_ALPHA
	#define GL_ONE_MINUS_CONSTANT_ALPHA       srGL_ONE_MINUS_CONSTANT_ALPHA
	#define GL_BLEND_COLOR                    srGL_BLEND_COLOR
	#define GL_FUNC_ADD                       srGL_FUNC_ADD
	#define GL_MIN                            srGL_MIN
	#define GL_MAX                            srGL_MAX
	#define GL_BLEND_EQUATION                 srGL_BLEND_EQUATION
	#define GL_FUNC_SUBTRACT                  srGL_FUNC_SUBTRACT
	#define GL_FUNC_REVERSE_SUBTRACT          srGL_FUNC_REVERSE_SUBTRACT
#endif

#define srGL_TEXTURE0                       0x84C0
#define srGL_TEXTURE1                       0x84C1
#define srGL_TEXTURE2                       0x84C2
#define srGL_TEXTURE3                       0x84C3
#define srGL_TEXTURE4                       0x84C4
#define srGL_TEXTURE5                       0x84C5
#define srGL_TEXTURE6                       0x84C6
#define srGL_TEXTURE7                       0x84C7
#define srGL_TEXTURE8                       0x84C8
#define srGL_TEXTURE9                       0x84C9
#define srGL_TEXTURE10                      0x84CA
#define srGL_TEXTURE11                      0x84CB
#define srGL_TEXTURE12                      0x84CC
#define srGL_TEXTURE13                      0x84CD
#define srGL_TEXTURE14                      0x84CE
#define srGL_TEXTURE15                      0x84CF
#define srGL_TEXTURE16                      0x84D0
#define srGL_TEXTURE17                      0x84D1
#define srGL_TEXTURE18                      0x84D2
#define srGL_TEXTURE19                      0x84D3
#define srGL_TEXTURE20                      0x84D4
#define srGL_TEXTURE21                      0x84D5
#define srGL_TEXTURE22                      0x84D6
#define srGL_TEXTURE23                      0x84D7
#define srGL_TEXTURE24                      0x84D8
#define srGL_TEXTURE25                      0x84D9
#define srGL_TEXTURE26                      0x84DA
#define srGL_TEXTURE27                      0x84DB
#define srGL_TEXTURE28                      0x84DC
#define srGL_TEXTURE29                      0x84DD
#define srGL_TEXTURE30                      0x84DE
#define srGL_TEXTURE31                      0x84DF
#define srGL_ACTIVE_TEXTURE                 0x84E0
#define srGL_MULTISAMPLE                    0x809D
#define srGL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define srGL_SAMPLE_ALPHA_TO_ONE            0x809F
#define srGL_SAMPLE_COVERAGE                0x80A0
#define srGL_SAMPLE_BUFFERS                 0x80A8
#define srGL_SAMPLES                        0x80A9
#define srGL_SAMPLE_COVERAGE_VALUE          0x80AA
#define srGL_SAMPLE_COVERAGE_INVERT         0x80AB
#define srGL_TEXTURE_CUBE_MAP               0x8513
#define srGL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define srGL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define srGL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define srGL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define srGL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define srGL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define srGL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define srGL_PROXY_TEXTURE_CUBE_MAP         0x851B
#define srGL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
#define srGL_COMPRESSED_RGB                 0x84ED
#define srGL_COMPRESSED_RGBA                0x84EE
#define srGL_TEXTURE_COMPRESSION_HINT       0x84EF
#define srGL_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define srGL_TEXTURE_COMPRESSED             0x86A1
#define srGL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define srGL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define srGL_CLAMP_TO_BORDER                0x812D
#ifdef SRGL_MOCK_GL
	#define GL_TEXTURE0                       srGL_TEXTURE0
	#define GL_TEXTURE1                       srGL_TEXTURE1
	#define GL_TEXTURE2                       srGL_TEXTURE2
	#define GL_TEXTURE3                       srGL_TEXTURE3
	#define GL_TEXTURE4                       srGL_TEXTURE4
	#define GL_TEXTURE5                       srGL_TEXTURE5
	#define GL_TEXTURE6                       srGL_TEXTURE6
	#define GL_TEXTURE7                       srGL_TEXTURE7
	#define GL_TEXTURE8                       srGL_TEXTURE8
	#define GL_TEXTURE9                       srGL_TEXTURE9
	#define GL_TEXTURE10                      srGL_TEXTURE10
	#define GL_TEXTURE11                      srGL_TEXTURE11
	#define GL_TEXTURE12                      srGL_TEXTURE12
	#define GL_TEXTURE13                      srGL_TEXTURE13
	#define GL_TEXTURE14                      srGL_TEXTURE14
	#define GL_TEXTURE15                      srGL_TEXTURE15
	#define GL_TEXTURE16                      srGL_TEXTURE16
	#define GL_TEXTURE17                      srGL_TEXTURE17
	#define GL_TEXTURE18                      srGL_TEXTURE18
	#define GL_TEXTURE19                      srGL_TEXTURE19
	#define GL_TEXTURE20                      srGL_TEXTURE20
	#define GL_TEXTURE21                      srGL_TEXTURE21
	#define GL_TEXTURE22                      srGL_TEXTURE22
	#define GL_TEXTURE23                      srGL_TEXTURE23
	#define GL_TEXTURE24                      srGL_TEXTURE24
	#define GL_TEXTURE25                      srGL_TEXTURE25
	#define GL_TEXTURE26                      srGL_TEXTURE26
	#define GL_TEXTURE27                      srGL_TEXTURE27
	#define GL_TEXTURE28                      srGL_TEXTURE28
	#define GL_TEXTURE29                      srGL_TEXTURE29
	#define GL_TEXTURE30                      srGL_TEXTURE30
	#define GL_TEXTURE31                      srGL_TEXTURE31
	#define GL_ACTIVE_TEXTURE                 srGL_ACTIVE_TEXTURE
	#define GL_MULTISAMPLE                    srGL_MULTISAMPLE
	#define GL_SAMPLE_ALPHA_TO_COVERAGE       srGL_SAMPLE_ALPHA_TO_COVERAGE
	#define GL_SAMPLE_ALPHA_TO_ONE            srGL_SAMPLE_ALPHA_TO_ONE
	#define GL_SAMPLE_COVERAGE                srGL_SAMPLE_COVERAGE
	#define GL_SAMPLE_BUFFERS                 srGL_SAMPLE_BUFFERS
	#define GL_SAMPLES                        srGL_SAMPLES
	#define GL_SAMPLE_COVERAGE_VALUE          srGL_SAMPLE_COVERAGE_VALUE
	#define GL_SAMPLE_COVERAGE_INVERT         srGL_SAMPLE_COVERAGE_INVERT
	#define GL_TEXTURE_CUBE_MAP               srGL_TEXTURE_CUBE_MAP
	#define GL_TEXTURE_BINDING_CUBE_MAP       srGL_TEXTURE_BINDING_CUBE_MAP
	#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    srGL_TEXTURE_CUBE_MAP_POSITIVE_X
	#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    srGL_TEXTURE_CUBE_MAP_NEGATIVE_X
	#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    srGL_TEXTURE_CUBE_MAP_POSITIVE_Y
	#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    srGL_TEXTURE_CUBE_MAP_NEGATIVE_Y
	#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    srGL_TEXTURE_CUBE_MAP_POSITIVE_Z
	#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    srGL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	#define GL_PROXY_TEXTURE_CUBE_MAP         srGL_PROXY_TEXTURE_CUBE_MAP
	#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      srGL_MAX_CUBE_MAP_TEXTURE_SIZE
	#define GL_COMPRESSED_RGB                 srGL_COMPRESSED_RGB
	#define GL_COMPRESSED_RGBA                srGL_COMPRESSED_RGBA
	#define GL_TEXTURE_COMPRESSION_HINT       srGL_TEXTURE_COMPRESSION_HINT
	#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE  srGL_TEXTURE_COMPRESSED_IMAGE_SIZE
	#define GL_TEXTURE_COMPRESSED             srGL_TEXTURE_COMPRESSED
	#define GL_NUM_COMPRESSED_TEXTURE_FORMATS srGL_NUM_COMPRESSED_TEXTURE_FORMATS
	#define GL_COMPRESSED_TEXTURE_FORMATS     srGL_COMPRESSED_TEXTURE_FORMATS
	#define GL_CLAMP_TO_BORDER                srGL_CLAMP_TO_BORDER
#endif

#define srGL_BLEND_DST_RGB                  0x80C8
#define srGL_BLEND_SRC_RGB                  0x80C9
#define srGL_BLEND_DST_ALPHA                0x80CA
#define srGL_BLEND_SRC_ALPHA                0x80CB
#define srGL_POINT_FADE_THRESHOLD_SIZE      0x8128
#define srGL_DEPTH_COMPONENT16              0x81A5
#define srGL_DEPTH_COMPONENT24              0x81A6
#define srGL_DEPTH_COMPONENT32              0x81A7
#define srGL_MIRRORED_REPEAT                0x8370
#define srGL_MAX_TEXTURE_LOD_BIAS           0x84FD
#define srGL_TEXTURE_LOD_BIAS               0x8501
#define srGL_INCR_WRAP                      0x8507
#define srGL_DECR_WRAP                      0x8508
#define srGL_TEXTURE_DEPTH_SIZE             0x884A
#define srGL_TEXTURE_COMPARE_MODE           0x884C
#define srGL_TEXTURE_COMPARE_FUNC           0x884D
#ifdef SRGL_MOCK_GL
	#define GL_BLEND_DST_RGB                  srGL_BLEND_DST_RGB
	#define GL_BLEND_SRC_RGB                  srGL_BLEND_SRC_RGB
	#define GL_BLEND_DST_ALPHA                srGL_BLEND_DST_ALPHA
	#define GL_BLEND_SRC_ALPHA                srGL_BLEND_SRC_ALPHA
	#define GL_POINT_FADE_THRESHOLD_SIZE      srGL_POINT_FADE_THRESHOLD_SIZE
	#define GL_DEPTH_COMPONENT16              srGL_DEPTH_COMPONENT16
	#define GL_DEPTH_COMPONENT24              srGL_DEPTH_COMPONENT24
	#define GL_DEPTH_COMPONENT32              srGL_DEPTH_COMPONENT32
	#define GL_MIRRORED_REPEAT                srGL_MIRRORED_REPEAT
	#define GL_MAX_TEXTURE_LOD_BIAS           srGL_MAX_TEXTURE_LOD_BIAS
	#define GL_TEXTURE_LOD_BIAS               srGL_TEXTURE_LOD_BIAS
	#define GL_INCR_WRAP                      srGL_INCR_WRAP
	#define GL_DECR_WRAP                      srGL_DECR_WRAP
	#define GL_TEXTURE_DEPTH_SIZE             srGL_TEXTURE_DEPTH_SIZE
	#define GL_TEXTURE_COMPARE_MODE           srGL_TEXTURE_COMPARE_MODE
	#define GL_TEXTURE_COMPARE_FUNC           srGL_TEXTURE_COMPARE_FUNC
#endif

#define srGL_BUFFER_SIZE                    0x8764
#define srGL_BUFFER_USAGE                   0x8765
#define srGL_QUERY_COUNTER_BITS             0x8864
#define srGL_CURRENT_QUERY                  0x8865
#define srGL_QUERY_RESULT                   0x8866
#define srGL_QUERY_RESULT_AVAILABLE         0x8867
#define srGL_ARRAY_BUFFER                   0x8892
#define srGL_ELEMENT_ARRAY_BUFFER           0x8893
#define srGL_ARRAY_BUFFER_BINDING           0x8894
#define srGL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
#define srGL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define srGL_READ_ONLY                      0x88B8
#define srGL_WRITE_ONLY                     0x88B9
#define srGL_READ_WRITE                     0x88BA
#define srGL_BUFFER_ACCESS                  0x88BB
#define srGL_BUFFER_MAPPED                  0x88BC
#define srGL_BUFFER_MAP_POINTER             0x88BD
#define srGL_STREAM_DRAW                    0x88E0
#define srGL_STREAM_READ                    0x88E1
#define srGL_STREAM_COPY                    0x88E2
#define srGL_STATIC_DRAW                    0x88E4
#define srGL_STATIC_READ                    0x88E5
#define srGL_STATIC_COPY                    0x88E6
#define srGL_DYNAMIC_DRAW                   0x88E8
#define srGL_DYNAMIC_READ                   0x88E9
#define srGL_DYNAMIC_COPY                   0x88EA
#define srGL_SAMPLES_PASSED                 0x8914
#ifdef SRGL_MOCK_GL
	#define GL_BUFFER_SIZE                    srGL_BUFFER_SIZE
	#define GL_BUFFER_USAGE                   srGL_BUFFER_USAGE
	#define GL_QUERY_COUNTER_BITS             srGL_QUERY_COUNTER_BITS
	#define GL_CURRENT_QUERY                  srGL_CURRENT_QUERY
	#define GL_QUERY_RESULT                   srGL_QUERY_RESULT
	#define GL_QUERY_RESULT_AVAILABLE         srGL_QUERY_RESULT_AVAILABLE
	#define GL_ARRAY_BUFFER                   srGL_ARRAY_BUFFER
	#define GL_ELEMENT_ARRAY_BUFFER           srGL_ELEMENT_ARRAY_BUFFER
	#define GL_ARRAY_BUFFER_BINDING           srGL_ARRAY_BUFFER_BINDING
	#define GL_ELEMENT_ARRAY_BUFFER_BINDING   srGL_ELEMENT_ARRAY_BUFFER_BINDING
	#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING srGL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING
	#define GL_READ_ONLY                      srGL_READ_ONLY
	#define GL_WRITE_ONLY                     srGL_WRITE_ONLY
	#define GL_READ_WRITE                     srGL_READ_WRITE
	#define GL_BUFFER_ACCESS                  srGL_BUFFER_ACCESS
	#define GL_BUFFER_MAPPED                  srGL_BUFFER_MAPPED
	#define GL_BUFFER_MAP_POINTER             srGL_BUFFER_MAP_POINTER
	#define GL_STREAM_DRAW                    srGL_STREAM_DRAW
	#define GL_STREAM_READ                    srGL_STREAM_READ
	#define GL_STREAM_COPY                    srGL_STREAM_COPY
	#define GL_STATIC_DRAW                    srGL_STATIC_DRAW
	#define GL_STATIC_READ                    srGL_STATIC_READ
	#define GL_STATIC_COPY                    srGL_STATIC_COPY
	#define GL_DYNAMIC_DRAW                   srGL_DYNAMIC_DRAW
	#define GL_DYNAMIC_READ                   srGL_DYNAMIC_READ
	#define GL_DYNAMIC_COPY                   srGL_DYNAMIC_COPY
	#define GL_SAMPLES_PASSED                 srGL_SAMPLES_PASSED
#endif

typedef uint32_t srGLbitfield;
typedef uint8_t  srGLboolean;
typedef int8_t   srGLbyte;
typedef float    srGLclampf;
typedef uint32_t srGLenum;
typedef float    srGLfloat;
typedef int32_t  srGLint;
typedef int16_t  srGLshort;
typedef int32_t  srGLsizei;
typedef uint8_t  srGLubyte;
typedef uint32_t srGLuint;
typedef uint16_t srGLushort;
typedef void     srGLvoid;
#ifdef SRGL_MOCK_GL
	typedef srGLbitfield GLbitfield;
	typedef srGLboolean GLboolean;
	typedef srGLbyte GLbyte;
	typedef srGLclampf GLclampf;
	typedef srGLenum GLenum;
	typedef srGLfloat GLfloat;
	typedef srGLint GLint;
	typedef srGLshort GLshort;
	typedef srGLsizei GLsizei;
	typedef srGLubyte GLubyte;
	typedef srGLuint GLuint;
	typedef srGLushort GLushort;
	typedef srGLvoid GLvoid;
#endif

typedef char     srGLchar;
#ifdef SRGL_MOCK_GL
	typedef srGLchar GLchar;
#endif

#if !defined(GL_ARB_shader_objects)
typedef char     srGLcharARB;
typedef void    *srGLhandleARB;
#ifdef SRGL_MOCK_GL
	typedef srGLcharARB GLcharARB;
	typedef srGLhandleARB GLhandleARB;
#endif
#endif

typedef double   srGLdouble;
typedef double   srGLclampd;
#ifdef SRGL_MOCK_GL
	typedef srGLdouble GLdouble;
	typedef srGLclampd GLclampd;
#endif

#if !defined(ARB_ES2_compatibility) && !defined(GL_VERSION_4_1)
typedef int32_t  srGLfixed;
#ifdef SRGL_MOCK_GL
	typedef srGLfixed GLfixed;
#endif
#endif

#if !defined(GL_ARB_half_float_vertex) && !defined(GL_VERSION_3_0)
typedef uint16_t srGLhalf;
#ifdef SRGL_MOCK_GL
	typedef srGLhalf GLhalf;
#endif
#endif

#if !defined(GL_ARB_half_float_pixel)
typedef uint16_t srGLhalfARB;
#ifdef SRGL_MOCK_GL
	typedef srGLhalfARB GLhalfARB;
#endif
#endif

#if !defined(GL_ARB_sync) && !defined(GL_VERSION_3_2)
typedef int64_t  srGLint64;
typedef struct __GLsync *srGLsync;
typedef uint64_t srGLuint64;
#ifdef SRGL_MOCK_GL
	typedef srGLint64 GLint64;
	typedef srGLsync GLsync;
	typedef srGLuint64 GLuint64;
#endif
#endif

#if !defined(GL_EXT_timer_query)
typedef int64_t  srGLint64EXT;
typedef uint64_t srGLuint64EXT;
#ifdef SRGL_MOCK_GL
	typedef srGLint64EXT GLint64EXT;
	typedef srGLuint64EXT GLuint64EXT;
#endif
#endif

typedef intptr_t srGLintptr;
typedef intptr_t srGLsizeiptr;
#ifdef SRGL_MOCK_GL
	typedef srGLintptr GLintptr;
	typedef srGLsizeiptr GLsizeiptr;
#endif

typedef intptr_t srGLintptrARB;
typedef intptr_t srGLsizeiptrARB;
#ifdef SRGL_MOCK_GL
	typedef srGLintptrARB GLintptrARB;
	typedef srGLsizeiptrARB GLsizeiptrARB;
#endif

#endif // glsr_types_h_
