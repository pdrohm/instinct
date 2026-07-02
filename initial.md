Act as a senior mobile product designer, React Native engineer, and interaction specialist.

I want you to analyze and improve the current editor experience in our Storia app.

Goal:
Rewrite and improve our current image editor so the user experience feels close to SCRL.

Main inspiration:
SCRL feels like a smooth, mobile first, touch based canvas editor for social posts and carousels. The experience feels simple, visual, playful, and highly responsive. Users can drag, pinch, zoom, rotate, resize, position images, adjust layouts, add text, stickers, backgrounds, and export social ready images without feeling like they are using a heavy design tool.

Before implementing anything, inspect the current codebase and understand how our editor is currently built.

Focus areas:

1. Current editor architecture

Find all screens, components, hooks, stores, gestures, canvas logic, image manipulation logic, export logic, templates, stickers, text tools, and media assets related to the current editor.

Identify:

What is already working

What feels fragile

What causes bad UX

What is hard to maintain

What can be reused

What should be rewritten

What editor options exist in the UI but are missing implementation

What editor tools exist in the code but are not exposed in the UI

What tools are partially implemented but broken or incomplete

2. SCRL like UX principles

Evaluate our editor against these principles:

Canvas first experience

Bottom toolbars or bottom sheets for editing tools

Direct manipulation on the canvas

Smooth drag, pinch, zoom, rotate, and resize gestures

Clear selected object state

Contextual controls when an object is selected

Template first creation flow

Fast photo replacement

Easy sticker and text placement

Social format preview while editing

Simple export flow

Low friction editing without too many nested screens

3. Gesture and interaction quality

Pay special attention to dragging, pinch to zoom, rotation, scaling, resizing, and object selection.

The editor should feel smooth and natural.

Analyze whether we should use or improve:

React Native Gesture Handler

React Native Reanimated

Skia

SVG

Image components

Canvas based rendering

Shared values for transforms

Gesture composition

Hit testing

Selection boxes

Bounding boxes

Snap guides

Layer ordering

The user should be able to:

Tap an object to select it

Drag it smoothly

Pinch to scale

Rotate naturally

Move around the canvas without lag

Resize without jumpy behavior

Keep gestures stable when switching fingers

Undo mistakes easily

Delete or duplicate selected elements

Bring elements forward or backward

Replace images easily

Edit selected text easily

Move stickers naturally

4. Editor UX flow

Propose a better flow inspired by SCRL.

Suggested flow:

Choose format or template

Add photos

Edit on canvas

Use bottom toolbar for tools

Tap an element for contextual editing

Preview final social output

Export cleanly

Think about the best mobile UX for:

Templates

Photo picker

Backgrounds

Frames

Stickers

Text

Fonts

Colors

Layer controls

Crop

Replace image

Export

Carousel or multi image output

5. Performance and reliability

The editor must feel fast on real devices.

Analyze possible performance issues:

Too many React rerenders

Gesture state stored in React state

Expensive image rendering

Large images not optimized

Unnecessary layout recalculations

Bad usage of animated values

Export blocking the UI thread

Memory usage during export

Complex nested views inside the canvas

Unstable gesture composition

Suggest a better technical approach that keeps the canvas smooth.

6. Product goal

The editor should help users create beautiful social posts quickly.

Do not make it feel like a complex design tool.

Prioritize:

Fast first result

Easy editing

Beautiful templates

Smooth gestures

Clear export

Mobile native feeling

Playful interactions

Visual simplicity

7. Missing editor tools and options

After analyzing the current codebase, identify all missing editor options related to the editor and tools.

Look specifically for:

Buttons that do not do anything

Disabled tools

Hidden tools

Incomplete toolbar actions

Missing bottom sheet options

Missing contextual controls

Missing text editing options

Missing sticker editing options

Missing image editing options

Missing background options

Missing template options

Missing export options

Missing crop or replace flows

Missing delete, duplicate, reorder, undo, redo, bring forward, send backward actions

Missing carousel or multi page controls

Missing selection controls

Missing resizing controls

Missing snapping or alignment helpers

Missing preview controls

For each missing option, explain:

Where it appears in the code

What seems intended

What is currently missing

What files need to change

Whether it can be implemented now

Whether it depends on design, backend, assets, or product decisions

8. Implementation request for Fable

After the analysis, implement the missing editor options that are already supported or implied by the current codebase.

Use the existing architecture when it makes sense.

You may refactor or rebuild the editor from scratch if the current implementation is too fragile or limiting.

First, analyze whether the current editor can be safely improved or whether a fresh implementation would produce a better long term result.

If rebuilding from scratch is the best path, explain why, define the new architecture, and implement it in staged steps so the app remains stable.

The priority is not preserving the current editor structure. The priority is creating a smooth, reliable, SCRL like editing experience.

Do not invent unrelated features.

Do not add large new systems unless required.

Prioritize implementing missing or incomplete editor tools that the code already suggests should exist.

The implementation should focus on:

Making existing editor buttons work

Exposing hidden or unused editor tools when appropriate

Completing incomplete tool flows

Improving object selection

Improving drag, pinch, zoom, scale, resize, and rotate

Adding basic contextual actions for selected elements

Adding delete, duplicate, layer ordering, and replace where the architecture supports it

Improving the bottom toolbar and bottom sheet usability

Making the export flow more reliable

Keeping the UX smooth and close to SCRL

9. Output required before coding

Return your analysis in this structure:

Current editor summary

Files and code areas inspected

Current UX problems

Current technical problems

Existing editor tools found

Missing editor options found

Partially implemented tools found

SCRL inspired UX proposal

Recommended editor architecture

Gesture system proposal

Canvas and rendering proposal

Template and asset system proposal

Export flow proposal

Performance risks

Should we improve the current editor or rebuild from scratch?

Implementation plan for missing options

Small MVP version

Future improvements

Open questions

10. Implementation plan

After the analysis, propose an MVP rewrite plan that can be implemented safely.

The MVP should include:

A canvas screen

Add image

Select image

Drag image

Pinch to zoom

Rotate image

Resize image

Delete image

Duplicate image

Replace image

Layer ordering

Add text

Edit text

Move text

Delete text

Add sticker

Move sticker

Scale sticker

Delete sticker

Basic bottom toolbar

Contextual controls for selected elements

Export final image

Avoid huge rewrites unless justified.

However, if the current editor is not good enough, you may rebuild it from scratch.

If rebuilding from scratch, use a staged migration so the app remains stable.

11. Coding instructions

Once the analysis and plan are complete, start implementing the missing editor options in small safe steps.

For every change:

Respect the existing design system when possible

Reference the exact files being changed

Keep gestures smooth and stable

Avoid unnecessary rerenders

Prefer Reanimated shared values for gesture state

Avoid storing high frequency gesture state in React state

Keep the canvas interaction responsive

Do not break existing export behavior

Do not remove existing features unless clearly replaced

Do not start with a full rewrite unless the current implementation is not salvageable

If a full rebuild is better, explain the reasoning before coding

12. Important constraints

Respect the existing design system when possible.

Reference exact files, components, hooks, stores, and libraries found in the project.

Prefer smooth mobile gestures over complex feature depth.

The first priority is interaction quality.

The editor should feel delightful before it becomes feature complete.

Focus on features related to the editor and tools.

Implement missing options that the current code shows or implies.

You may remake the editor from scratch if the current implementation is not good enough.

If you choose to rebuild it, explain the reasoning, proposed architecture, migration plan, and risks before coding.

Do not implement unrelated product features.

Do not overcomplicate the first version.

The final experience should feel closer to SCRL: visual, smooth, playful, direct, and easy to use.
