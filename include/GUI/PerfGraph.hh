#ifndef PERFGRAPH_H
#define PERFGRAPH_H

#include "GUI/GUIObject.hh"

#include "GUI/nanovg.hh"
#include <functional>
#include <string>


#define GPU_QUERY_COUNT 5
enum GraphRenderStyle {
            GRAPH_RENDER_FPS,
            GRAPH_RENDER_MS,
            GRAPH_RENDER_PERCENT,
        };


class PerfGraph: public GUIObject {
    public:
        PerfGraph(GUIObject *parent, int style, std::string name);

        //~PerfGraph();

        

        void updateGraph(float frameTime);
        //void draw(NVGcontext* vg, float x, float y);
        float getGraphAverage();

        /*
        struct GPUtimer {
            int supported;
            int cur, ret;
            unsigned int queries[GPU_QUERY_COUNT];
        };
        typedef struct GPUtimer GPUtimer;

        void initGPUTimer(GPUtimer* timer);
        void startGPUTimer(GPUtimer* timer);
        int stopGPUTimer(GPUtimer* timer, float* times, int maxTimes);
        */

        //From button
        const std::string &caption() const { return mCaption; }
        void setCaption(const std::string &caption) { mCaption = caption; }

        const Color &backgroundColor() const { return mBackgroundColor; }
        void setBackgroundColor(const Color &backgroundColor) { mBackgroundColor = backgroundColor; }

        const Color &textColor() const { return mTextColor; }
        void setTextColor(const Color &textColor) { mTextColor = textColor; }

        int icon() const { return mIcon; }
        void setIcon(int icon) { mIcon = icon; }

        bool pushed() const { return mPushed; }
        void setPushed(bool pushed) { mPushed = pushed; }

        /// Set the push callback (for any type of button)
        std::function<void()> callback() const { return mCallback; }
        void setCallback(const std::function<void()> &callback) { mCallback = callback; }

        /// Set the change callback (for toggle buttons)
        std::function<void(bool)> changeCallback() const { return mChangeCallback; }
        void setChangeCallback(const std::function<void(bool)> &callback) { mChangeCallback = callback; }

        virtual bool mouseButtonEvent(const ivec2 &p, int button, bool down, int modifiers);
        virtual void draw(NVGcontext *ctx);
    private:
        int mStyle;
        
        int head;
        float* values;
        float prevt;
        //GPUtimer *gputimer;
    protected:
        std::string mCaption;
        int mIcon;
        bool mPushed, mEnabled = true;
        Color mBackgroundColor, mForegroundColor;
        Color mTextColor;
        std::function<void()> mCallback;
        std::function<void(bool)> mChangeCallback;
        int graphHistoryCount = 100;

};

#endif
