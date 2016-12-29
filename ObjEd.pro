INCLUDEPATH += $$quote(C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include)

CONFIG(release, debug|release): LIBS += -L$$quote(C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/) -ld3dx10
else:CONFIG(debug, debug|release): LIBS += -L$$quote(C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/) -ld3dx10d
LIBS += -L$$quote(C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/) -ld3d10
LIBS += -L$$quote(C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/) -ldxguid
LIBS += -L$$quote(C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/) -ldxgi


SOURCES += \
    main.cpp \
    App/SingleBox.cpp \
    App/RenderViewApp.cpp \
    App/pointlistmodel.cpp \
    App/pairtangentsmodel.cpp \
    App/ObjEdRenderApp.cpp \
    App/linelistmodel.cpp \
    App/imagequadmodel.cpp \
    App/GridModel.cpp \
    App/EditorTextureApp.cpp \
    App/EditableMeshModelProcTex.cpp \
    App/EditableMeshModel.cpp \
    App/beziermodel.cpp \
    App/BaseTextureDX.cpp \
    App/BaseDX.cpp \
    App/AxisModel.cpp \
    App/ObjEd/ObjRenderAppTextured.cpp \
    App/ObjEd/ObjRenderAppPhong.cpp \
    App/ObjEd/GizmoTMeshModel.cpp \
    Camera/Camera.cpp \
    Editor_Bezier/editorobject.cpp \
    Editor_Bezier/EditorGlobal.cpp \
    Editor_Bezier/editorbezierstate.cpp \
    Editor_Bezier/editorbezier.cpp \
    Editor_Bezier/bezier.cpp \
    Editor_DirGraph/DirGraphEditor.cpp \
    Editor_Texture/EditorMaterial.cpp \
    Editor_Texture/DataLayer/Signal.cpp \
    Editor_Texture/DataLayer/ProcShaderUnitTesting.cpp \
    Editor_Texture/DataLayer/ProcShaderMisc.cpp \
    Editor_Texture/DataLayer/ProcShaderIOLayer.cpp \
    Editor_Texture/DataLayer/ProcShader.cpp \
    Editor_Texture/DataLayer/Presets.cpp \
    Editor_Texture/DataLayer/LayerSignal.cpp \
    Editor_Texture/DataLayer/LayerOp.cpp \
    Editor_Texture/DataLayer/Layer.cpp \
    Editor_Texture/DataLayer/Identifiers.cpp \
    Editor_Texture/DataLayer/Channel.cpp \
    Editor_Texture/DataLayer/Channels/ChannelRgba.cpp \
    Editor_Texture/DataLayer/Channels/ChannelBump.cpp \
    Editor_Texture/DataLayer/IO/ProcShaderDisk.cpp \
    Editor_Texture/DataLayer/Library/ChannelLibrary.cpp \
    Editor_Texture/DataLayer/Signals/SignalStripsIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalStrips.cpp \
    Editor_Texture/DataLayer/Signals/SignalSpheresIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalSpheres.cpp \
    Editor_Texture/DataLayer/Signals/SignalNoiseSinIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalNoiseSin.cpp \
    Editor_Texture/DataLayer/Signals/SignalNoiseIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalNoise.cpp \
    Editor_Texture/DataLayer/Signals/SignalMarbleIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalMarble.cpp \
    Editor_Texture/DataLayer/Signals/SignalLinesIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalLines.cpp \
    Editor_Texture/DataLayer/Signals/SignalKhanIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalKhan.cpp \
    Editor_Texture/DataLayer/Signals/SignalIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalCurveVeinsIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalCurveVeins.cpp \
    Exterior/stringTokenizer.cpp \
    Exterior/bmp.cpp \
    Geometry/SimpleGraph.cpp \
    Geometry/MathConstants.cpp \
    Geometry/Intersections.cpp \
    Geometry/DxMathHelpers.cpp \
    Geometry/DxMeshModelIndexed.cpp \
    Geometry/DirGraph/SubRule.cpp \
    Geometry/DirGraph/Pattern.cpp \
    Geometry/DirGraph/DirGraph.cpp \
    Misc/StlHelpers.cpp \
    Noise/RandomNumberGenerator.cpp \
    Noise/haarNoise.cpp \
    Noise/Haar.cpp \
    ObjEditor/objeditorstate.cpp \
    ObjEditor/ObjEditorSharedData.cpp \
    ObjEditor/objectEditor.cpp \
    ObjEditor/Gizmos/GizmoT.cpp \
    ObjEditor/Gizmos/GizmoS.cpp \
    ObjEditor/Gizmos/GizmoR.cpp \
    ObjEditor/Mesh/HalfEdge/HE_vert.cpp \
    ObjEditor/Mesh/HalfEdge/HE_face.cpp \    
    ObjEditor/Mesh/HalfEdge/HE_edge.cpp \
    ObjEditor/Mesh/IO/OpStackIO.cpp \
    ObjEditor/Mesh/IO/OpInfoIO.cpp \
    ObjEditor/Mesh/IO/MeshIO.cpp \
    ObjEditor/Mesh/operations/ProcTex.cpp \
    ObjEditor/Mesh/operations/NormalScale.cpp \
    ObjEditor/Mesh/operations/KniveLine.cpp \
    ObjEditor/Mesh/operations/ExtrusionMatrix.cpp \
    ObjEditor/Mesh/operations/ExtrusionInner.cpp \
    ObjEditor/Mesh/operations/Extrusion.cpp \
    ObjEditor/Mesh/operations/BevelV.cpp \
    ObjEditor/Mesh/operations/BevelE.cpp \
    ObjEditor/Mesh/Primitives/CubePrim.cpp \
    ObjEditor/Mesh/Serialization/MeshSerial.cpp \
    ObjEditor/Mesh/Serialization/MeshHalfEdgeSerial.cpp \
    ObjEditor/Mesh/Serialization/MeshComponentSerial.cpp \
    ObjEditor/Mesh/vertex.cpp \
    ObjEditor/Mesh/OpStack.cpp \
    ObjEditor/Mesh/MeshHalfEdge.cpp \
    ObjEditor/Mesh/MeshComponent.cpp \
    ObjEditor/Mesh/Mesh.cpp \
    ObjEditor/Mesh/Face.cpp \
    ObjEditor/Mesh/editableobject.cpp \
    ObjEditor/Mesh/Edge.cpp \
    ObjEditor/Operations/objEd_operations.cpp \
    physics/Physics.cpp \
    Project_Dependant/StoredShadersTranslucent.cpp \
    Project_Dependant/StoredShadersPhong.cpp \
    Project_Dependant/StoredShadersGeneralEditor.cpp \
    Project_Dependant/StoredShaders.cpp \
    Shader/ShaderHelpers.cpp \
    Shader/Shader.cpp \
    Timing/Timing_windows_x64.cpp \
    UI/noteditorabout.cpp \
    UI/MainWindow.cpp \
    UI_Bezier/viewManager.cpp \
    UI_Bezier/TabBezierWindow.cpp \
    UI_Bezier/QD3DWidgetBezier.cpp \
    UI_Bezier/objecttree.cpp \
    UI_DirGraph/TabDirGEditor.cpp \
    UI_DirGraph/QD3DWidgetDirG.cpp \
    UI_DirGraph/ObjViewDG.cpp \
    UI_ObjEditor/TabObjEditor.cpp \
    UI_ObjEditor/QD3DWidgetObj.cpp \
    UI_ObjEditor/ObjView.cpp \
    UI_ObjEditor/ObjectTreeWidget.cpp \
    UI_ObjEditor/OpWidgets/opTextureWidget.cpp \
    UI_ObjEditor/OpWidgets/opknifewidget.cpp \
    UI_ObjEditor/OpWidgets/opextrudewidget.cpp \
    UI_Pal/TabPalWindow.cpp \
    UI_Pal/colourwidget.cpp \
    UI_Textures/TabTextureWindow.cpp \
    UI_Textures/SignalLayerWidgetProfile.cpp \
    UI_Textures/SignalLayerWidget.cpp \
    UI_Textures/QD3DWidgetTexture.cpp \    
    UI_Textures/MinorWidgets/SingleColourWidget.cpp \
    UI_Textures/MinorWidgets/SignalNameButton.cpp \
    UI_Textures/MinorWidgets/LoadChannelDialog.cpp \
    UI_Textures/MinorWidgets/Gradient2Widget.cpp \
    UI_Textures/MinorWidgets/ChannelWidget.cpp \
    UI_Textures/MinorWidgets/ChannelNameButton.cpp \
    UI_Textures/MinorWidgets/ChannelButton.cpp \
    UI_Wavelet/TabWaveWindow.cpp \
    App/DirGraphEdRenderApp.cpp \
    App/EditableDirGraph.cpp \
    UI_DirGraph/MinorWidgets/NameButton.cpp \
    Editor_Texture/DataLayer/Signals/SignalTiledTrianglesIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalTiledTriangles.cpp \
    UI_Textures/MinorWidgets/ChannelSpecWidget.cpp \
    UI_Textures/MinorWidgets/GradientRenderer.cpp \
    Editor_Texture/DataLayer/Signals/SignalIdentityIO.cpp \
    Editor_Texture/DataLayer/Signals/SignalIdentity.cpp \
    UI_Textures/MinorWidgets/SplineNWidget.cpp



RESOURCES += \
    resources.qrc

HEADERS += \
    App/SingleBox.hpp \
    App/RenderViewApp.hpp \
    App/pointlistmodel.h \
    App/pairtangentsmodel.h \
    App/ObjEdRenderApp.hpp \
    App/linelistmodel.h \
    App/imagequadmodel.hpp \
    App/GridModel.h \
    App/EditorTextureApp.hpp \
    App/EditableMeshModelProcTex.h \
    App/EditableMeshModel.h \
    App/dx_misc.hpp \
    App/beziermodel.h \
    App/BaseTextureDX.hpp \
    App/BaseDX.hpp \
    App/AxisModel.h \
    App/ObjEd/GizmoTMeshModel.hpp \
    Camera/Camera.hpp \
    Editor_Bezier/editorobject.h \
    Editor_Bezier/EditorGlobal.hpp \
    Editor_Bezier/editorbezierstate.h \
    Editor_Bezier/editorbezier.h \
    Editor_Bezier/bezier.h \
    Editor_DirGraph/DirGraphEditor.hpp \
    Editor_Texture/EditorMaterial.hpp \
    Editor_Texture/DataLayer/Signal.hpp \
    Editor_Texture/DataLayer/ProcShaderUnitTesting.hpp \
    Editor_Texture/DataLayer/ProcShaderTextData.hpp \
    Editor_Texture/DataLayer/ProcShaderMisc.hpp \
    Editor_Texture/DataLayer/ProcShader.hpp \
    Editor_Texture/DataLayer/Presets.hpp \
    Editor_Texture/DataLayer/Layer.hpp \
    Editor_Texture/DataLayer/Identifiers.hpp \
    Editor_Texture/DataLayer/Channel.hpp \
    Editor_Texture/DataLayer/IO/ProcShaderDisk.hpp \
    Editor_Texture/DataLayer/Library/ChannelLibrary.hpp \
    Editor_Texture/DataLayer/Signals/SignalStrips.hpp \
    Editor_Texture/DataLayer/Signals/SignalSpheres.hpp \
    Editor_Texture/DataLayer/Signals/SignalNoiseSin.hpp \
    Editor_Texture/DataLayer/Signals/SignalNoise.hpp \
    Editor_Texture/DataLayer/Signals/SignalMarble.hpp \
    Editor_Texture/DataLayer/Signals/SignalLines.hpp \
    Editor_Texture/DataLayer/Signals/SignalKhan.hpp \
    Editor_Texture/DataLayer/Signals/SignalCurveVeins.hpp \
    Exterior/bmp.hpp \
    GameBall/TrackBp.hpp \
    GameBall/GameBp.hpp \
    Geometry/SimpleGraph.hpp \
    Geometry/MathConstants.hpp \
    Geometry/Intersections.hpp \
    Geometry/DxMeshModelIndexed.hpp \
    Geometry/DxMeshModel.hpp \
    Geometry/DxMathHelpers.hpp \
    Geometry/DirGraph/SubRule.hpp \
    Geometry/DirGraph/Pattern.hpp \
    Geometry/DirGraph/DirGraphDataTypes.hpp \
    Geometry/DirGraph/DirGraph.hpp \
    Misc/StlHelpers.hpp \
    Noise/RandomNumberGenerator.hpp \
    Noise/Haar.hpp \
    ObjEditor/objeditorstate.h \
    ObjEditor/ObjEditorSharedData.hpp \
    ObjEditor/objectEditor.h \
    ObjEditor/Gizmos/GizmoT.hpp \
    ObjEditor/Gizmos/GizmoS.hpp \
    ObjEditor/Gizmos/GizmoR.hpp \
    ObjEditor/Mesh/HalfEdge/HE_vert.hpp \
    ObjEditor/Mesh/HalfEdge/HE_face.hpp \
    ObjEditor/Mesh/HalfEdge/HE_edge.hpp \
    ObjEditor/Mesh/Serialization/MeshSerial.hpp \
    ObjEditor/Mesh/Serialization/MeshHalfEdgeSerial.hpp \
    ObjEditor/Mesh/Serialization/MeshComponentSerial.hpp \
    ObjEditor/Mesh/vertex.hpp \
    ObjEditor/Mesh/OpStack.hpp \
    ObjEditor/Mesh/OpInfo.hpp \
    ObjEditor/Mesh/OperationsOptions.hpp \
    ObjEditor/Mesh/OperationsInfo.hpp \
    ObjEditor/Mesh/MeshHalfEdge.hpp \
    ObjEditor/Mesh/MeshComponent.hpp \
    ObjEditor/Mesh/Mesh.hpp \
    ObjEditor/Mesh/Face.hpp \
    ObjEditor/Mesh/editableobject.h \
    ObjEditor/Mesh/Edge.hpp \        
    physics/Physics.hpp \
    Project_Dependant/StoredShaders.hpp \
    Shader/VertexLayout.hpp \
    Shader/ShaderHelpers.hpp \
    Shader/Shader.hpp \
    Timing/Timing_windows_x64.hpp \
    UI/noteditorabout.h \
    UI/MainWindow.hpp \
    UI_Bezier/viewManager.h \
    UI_Bezier/TabBezierWindow.h \
    UI_Bezier/QD3DWidgetBezier.hpp \
    UI_Bezier/objecttree.h \
    UI_DirGraph/TabDirGEditor.h \
    UI_DirGraph/QD3DWidgetDirG.hpp \
    UI_DirGraph/ObjViewDG.h \
    UI_ObjEditor/TabObjEditor.h \
    UI_ObjEditor/QD3DWidgetObj.hpp \
    UI_ObjEditor/ObjView.h \
    UI_ObjEditor/ObjectTreeWidget.h \
    UI_ObjEditor/OpWidgets/opTextureWidget.h \
    UI_ObjEditor/OpWidgets/opknifewidget.h \
    UI_ObjEditor/OpWidgets/opextrudewidget.h \
    UI_Pal/TabPalWindow.h \
    UI_Pal/colourwidget.h \
    UI_Textures/TabTextureWindow.hpp \
    UI_Textures/SignalLayerWidget.hpp \
    UI_Textures/QD3DWidgetTexture.hpp \
    UI_Textures/MinorWidgets/SingleColourWidget.h \
    UI_Textures/MinorWidgets/SignalNameButton.hpp \
    UI_Textures/MinorWidgets/LoadChannelDialog.hpp \
    UI_Textures/MinorWidgets/Gradient2Widget.hpp \
    UI_Textures/MinorWidgets/ChannelWidget.hpp \
    UI_Textures/MinorWidgets/ChannelNameButton.hpp \
    UI_Textures/MinorWidgets/ChannelButton.hpp \
    UI_Wavelet/TabWaveWindow.h \
    App/DirGraphEdRenderApp.hpp \
    App/EditableDirGraph.h \
    UI_DirGraph/MinorWidgets/NameButton.hpp \
    Editor_Texture/DataLayer/Signals/SignalTiledTriangles.hpp \
    UI_Textures/MinorWidgets/ChannelSpecWidget.hpp \
    UI_Textures/MinorWidgets/GradientRenderer.h \
    Editor_Texture/DataLayer/Signals/SignalIdentity.hpp \
    UI_Textures/MinorWidgets/SplineNWidget.hpp


































































































