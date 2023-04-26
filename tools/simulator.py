import dearpygui.dearpygui as dpg

height = 200
width = 300

def start_gui():
    dpg.create_context()
    dpg.create_viewport(title="Simulator", width=int(width * 1.1), height=int(height * 1.1), resizable=False)
    dpg.setup_dearpygui()

    with dpg.window(label="Main", width=width, height=height, no_resize=True, no_move=True, no_close=True):
        dpg.add_button(label="Hello world")

    dpg.show_viewport()
    dpg.start_dearpygui()
    dpg.destroy_context()

if __name__ == "__main__":
    start_gui()