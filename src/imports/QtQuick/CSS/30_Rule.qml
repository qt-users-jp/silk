/* Copyright (c) 2012 Silk Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Silk nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL SILK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick.CSS 3.0

CSSRule {
    // http://www.w3.org/Style/CSS/current-work

    /* CSS Color Module Level 3 */
    property string color
    property string opacity

    /* CSS Backgrounds and Borders Module Level 3 */
    property string background_color
    property string background_image
    property string background_repeat
    property string background_attachment
    property string background_option
    property string background_clip
    property string background_origin
    property string background_size
    property string background

    property string border_top_color
    property string border_right_color
    property string border_bottom_color
    property string border_left_color
    property string border_color

    property string border_top_style
    property string border_right_style
    property string border_bottom_style
    property string border_left_style
    property string border_style

    property string border_top_width
    property string border_right_width
    property string border_bottom_width
    property string border_left_width
    property string border_width

    property string border_top
    property string border_right
    property string border_bottom
    property string border_left
    property string border

    property string border_top_left_radius
    property string border_top_right_radius
    property string border_bottom_right_radius
    property string border_bottom_left_radius
    property string border_radius

    property string border_image_source
    property string border_image_slice
    property string border_image_width
    property string border_image_outset
    property string border_image_repeat
    property string border_image

    property string border_decoration_break
    property string box_shadow

    /* CSS Marquee Module Level 3 */
    property string overflow_style
    property string marquee_style
    property string marquee_play_count
    property string marquee_direction
    property string marquee_speed

    /* CSS Multi-column Layout Module */
    property string column_width
    property string column_count
    property string columns
    property string column_gap
    property string column_rule_color
    property string column_rule_style
    property string column_rule_width
    property string column_rule
    property string break_before
    property string break_after
    property string break_inside
    property string column_span
    property string column_fill

    /* CSS Flexible Box Layout Module */
    property string display
    property string min_width
    property string min_height
    property string flex_direction
    property string flex_wrap
    property string flex_flow
    property string order
    property string flex
    property string flex_grow
    property string flex_shrink
    property string flex_basis
    property string justify_content
    property string align_items
    property string align_self
    property string align_content

    /* CSS Speech Module */
    property string voice_volume
    property string voice_balance
    property string speak
    property string speak_as
    property string pause_before
    property string pause_after
    property string pause
    property string rest_before
    property string rest_after
    property string rest
    property string cue_before
    property string cue_after
    property string cue
    property string voice_family
    property string voice_rate
    property string voice_pitch
    property string voice_range
    property string voice_stress
    property string voice_duration

    /* CSS Animations */
    property string animation_name
    property string animation_duration
    property string animation_timing_function
    property string animation_iteration_count
    property string animation_direction
    property string animation_play_state
    property string animation_delay
    property string animation_fill_mode
    property string animation

    /* CSS Transforms */
    property string transform
    property string transform_origin
    property string transform_style
    property string perspective
    property string perspective_origin
    property string backface_visibility

    /* CSS Transitions */
    property string transition_property
    property string transition_duration
    property string transition_timing_function
    property string transition_delay
    property string transition

    /* CSS Basic User Interface Module Level 3 */
    property string content
    property string icon
    property string box_sizing
    property string outline
    property string outline_width
    property string outline_style
    property string outline_color
    property string outline_offset
    property string resize
    property string text_overflow
    property string cursor
    property string nav_index
    property string nav_up
    property string nav_right
    property string nav_down
    property string nav_left
    property string ime_mode

    /* CSS Writing Modes Module Level 3 */
    property string direction
    property string unicode_bidi
    property string writing_mode
    property string text_orientation
    property string text_combine_horizontal
    property string text_combine_mode

    /* CSS Fonts Module Level 3 */
    property string font_family
    property string font_weight
    property string font_stretch
    property string font_style
    property string font_size
    property string font_size_adjust
    property string font
    property string font_synthesis
    property string src
    property string font_style
    property string unicode_range
    property string font_variant
    property string font_feature_settings
    property string font_kerning
    property string font_variant_ligatures
    property string font_variant_position
    property string font_variant_caps
    property string font_variant_numeric
    property string font_variant_alternates
    property string font_variant_east_asian
    property string font_language_override

    /* CSS Regions Module Level 3 */
    property string flow_into
    property string flow_from
//    property string break_before
//    property string break_after
//    property string break_inside
    property string region_overflow

    /* CSS Text Level 3 */
    property string text_transform
    property string white_space
    property string tab_size
    property string line_break
    property string word_break
    property string hyphens
    property string overflow_wrap
    property string word_wrap
    property string text_align
    property string text_align_last
    property string text_jastify
    property string word_spacing
    property string letter_spacing
    property string text_indent
    property string hanging_punctuation

    property string text_decoration_line
    property string text_decoration_color
    property string text_decoration_style
    property string text_decoration
    property string text_decoration_skip
    property string text_underline_position
    property string text_emphasis_style
    property string text_emphasis_color
    property string text_emphasis
    property string text_emphasis_position
    property string text_shadow

    /* CSS Fragmentation Module Level 3 */
    property string orphans
    property string widows
//    property string break_before
//    property string break_after
//    property string break_inside
}

