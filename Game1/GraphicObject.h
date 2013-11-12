#pragma once
#ifndef __graphicobject_h__
#define __graphicobject_h__

// 
// Base object for all objects which needs to be drawed on the screen.
// 
class GraphicObject
{
public:
    //
    virtual ~GraphicObject() {};
    // 
    virtual void Draw() = 0;
    // 
    virtual void Update(int time, int delta = 0) = 0;
};

#endif /* __graphicobject_h__ */

