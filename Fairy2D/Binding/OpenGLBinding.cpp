#include "GLCommon.h"
#include "OpenGLBinding.h"
#include "LuaBinding.h"

namespace LuaBinding
{
    LUA_NEW(BlendColor)
    LUA_NEW(BlendEquation)
    LUA_NEW(BlendEquationSeparate)
    LUA_NEW(BlendFunc)
    LUA_NEW(BlendFuncSeparate)
    LUA_NEW(Clear)
    LUA_NEW(ClearColor)
    LUA_NEW(ClearDepth)
    LUA_NEW(ClearStencil)
    LUA_NEW(ColorMask)
    LUA_NEW(CullFace)
    LUA_NEW(DepthFunc)
    LUA_NEW(DepthMask)
    LUA_NEW(DepthRange)
    LUA_NEW(Disable)
    LUA_NEW(Enable)
    LUA_NEW(Finish)
    LUA_NEW(Flush)
    LUA_NEW(FrontFace)
    LUA_NEW(Hint)
    LUA_NEW(IsEnabled)
    LUA_NEW(LineWidth)
    LUA_NEW(LogicOp)
    LUA_NEW(PointSize)
    LUA_NEW(PolygonMode)
    LUA_NEW(PolygonOffset)
    LUA_NEW(Scissor)
    LUA_NEW(StencilFunc)
    LUA_NEW(StencilFuncSeparate)
    LUA_NEW(StencilMask)
    LUA_NEW(StencilMaskSeparate)
    LUA_NEW(StencilOp)
    LUA_NEW(StencilOpSeparate)

    void SetupOpenGLBinding(lua_State* state)
    {
        luaL_Reg regs[] = {
            LUA_REG(BlendColor)
            LUA_REG(BlendEquation)
            LUA_REG(BlendEquationSeparate)
            LUA_REG(BlendFunc)
            LUA_REG(BlendFuncSeparate)
            LUA_REG(Clear)
            LUA_REG(ClearColor)
            LUA_REG(ClearDepth)
            LUA_REG(ClearStencil)
            LUA_REG(ColorMask)
            LUA_REG(CullFace)
            LUA_REG(DepthFunc)
            LUA_REG(DepthMask)
            LUA_REG(DepthRange)
            LUA_REG(Disable)
            LUA_REG(Enable)
            LUA_REG(Finish)
            LUA_REG(Flush)
            LUA_REG(FrontFace)
            LUA_REG(Hint)
            LUA_REG(IsEnabled)
            LUA_REG(LineWidth)
            LUA_REG(LogicOp)
            LUA_REG(PointSize)
            LUA_REG(PolygonMode)
            LUA_REG(PolygonOffset)
            LUA_REG(Scissor)
            LUA_REG(StencilFunc)
            LUA_REG(StencilFuncSeparate)
            LUA_REG(StencilMask)
            LUA_REG(StencilMaskSeparate)
            LUA_REG(StencilOp)
            LUA_REG(StencilOpSeparate)
        };

        for (int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
            lua_register(state, regs[i].name, regs[i].func);
        }
    }
}