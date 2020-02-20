package sample;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.io.*;
import java.net.Socket;
import java.nio.charset.Charset;

class GUI extends JFrame {
    JFrame menuFrame;
    JTextArea T1, T2, T3, T4, T5;
    JTextArea L1, L2, L3, L4, L5;
    JTextArea Tczas, Twyniki;
    JButton B1 ,B2;

    public GUI() {}

    public void nowaGra(){
        B2.setEnabled(true);
        B1.setEnabled(false);

        T1.setEditable(false);
        T2.setEditable(false);
        T3.setEditable(false);
        T4.setEditable(false);
        T5.setEditable(false);

        Tczas.setText(null);
        T1.setText(null);
        T2.setText(null);
        T3.setText(null);
        T4.setText(null);
        T5.setText(null);

        L1.setText(null);
        L2.setText(null);
        L3.setText(null);
        L4.setText(null);
        L5.setText(null);

        Twyniki.setVisible(false);

        B2.setText("Gotowy");

    }

    public void wyczyscWyniki(){
        Twyniki.setText(null);
    }

    public void wyswietlWyniki(String s){
        Twyniki.append(s);
        Twyniki.setVisible(true);
    }
    public void wyswietlCzas (String s){
        Tczas.setText(s);
    }

    public void zmianaLitery(char c){
        B2.setEnabled(false);
        B1.setEnabled(true);
        String s = Character.toString(c).toUpperCase();

        T1.setEditable(true);
        T2.setEditable(true);
        T3.setEditable(true);
        T4.setEditable(true);
        T5.setEditable(true);

        Tczas.setText(null);
        T1.setText(null);
        T2.setText(null);
        T3.setText(null);
        T4.setText(null);
        T5.setText(null);

        L1.setText(s);
        L2.setText(s);
        L3.setText(s);
        L4.setText(s);
        L5.setText(s);


    }
    public void Menu(Socket sock){
        menuFrame = new JFrame("JTPP");
        menuFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        menuFrame.pack();

        SpringLayout layout = new SpringLayout();
        Container content = menuFrame.getContentPane();
        content.setLayout(layout);

        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        int ScWidth = screenSize.width/2;
        int ScHeight = 2*screenSize.height/8;
        menuFrame.setSize(ScWidth,ScHeight);
        menuFrame.setLocationRelativeTo(null);
        menuFrame.setResizable(false);
        menuFrame.setBackground(Color.white);


        Spring xSpring = Spring.constant(30,40,50);
        Spring ySpring = Spring.constant(20,30, 40);
        Spring y2Spring = Spring.constant(5,5, 5);

        JTextArea TT = new JTextArea("Państwa Miasta");
        TT.setFont(new Font("default", Font.ITALIC + Font.BOLD, 32));
        TT.setEditable(false);


        T1 = new JTextArea();
        T1.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_TAB) {
                    if (e.getModifiers() > 0) {
                        T1.transferFocusBackward();
                    } else {
                        T1.transferFocus();
                    }
                    e.consume();
                }
            }
            @Override
            public void keyTyped(KeyEvent e) {
                int max = 25;
                if(T1.getText().length() > max+1) {
                    e.consume();
                    String shortened = T1.getText().substring(0, max);
                    T1.setText(shortened);
                }else if(T1.getText().length() > max) {
                    e.consume();
                }
            }
        });


        T2 = new JTextArea();
        T2.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_TAB) {
                    if (e.getModifiers() > 0) {
                        T2.transferFocusBackward();
                    } else {
                        T2.transferFocus();
                    }
                    e.consume();
                }
            }
            @Override
            public void keyTyped(KeyEvent e) {
                int max = 25;
                if(T2.getText().length() > max+1) {
                    e.consume();
                    String shortened = T2.getText().substring(0, max);
                    T2.setText(shortened);
                }else if(T2.getText().length() > max) {
                    e.consume();
                }
            }
        });
        T3 = new JTextArea();
        T3.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_TAB) {
                    if (e.getModifiers() > 0) {
                        T3.transferFocusBackward();
                    } else {
                        T3.transferFocus();
                    }
                    e.consume();
                }
            }
            @Override
            public void keyTyped(KeyEvent e) {
                int max = 25;
                if(T3.getText().length() > max+1) {
                    e.consume();
                    String shortened = T3.getText().substring(0, max);
                    T3.setText(shortened);
                }else if(T3.getText().length() > max) {
                    e.consume();
                }
            }
        });
        T4 = new JTextArea();
        T4.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_TAB) {
                    if (e.getModifiers() > 0) {
                        T4.transferFocusBackward();
                    } else {
                        T4.transferFocus();
                    }
                    e.consume();
                }
            }
            @Override
            public void keyTyped(KeyEvent e) {
                int max = 25;
                if(T4.getText().length() > max+1) {
                    e.consume();
                    String shortened = T4.getText().substring(0, max);
                    T4.setText(shortened);
                }else if(T4.getText().length() > max) {
                    e.consume();
                }
            }
        });
        T5 = new JTextArea();
        T5.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_TAB) {
                    if (e.getModifiers() > 0) {
                        T5.transferFocusBackward();
                    } else {
                        T5.transferFocus();
                    }
                    e.consume();
                }
            }
            @Override
            public void keyTyped(KeyEvent e) {
                int max = 25;
                if(T5.getText().length() > max+1) {
                    e.consume();
                    String shortened = T5.getText().substring(0, max);
                    T5.setText(shortened);
                }else if(T5.getText().length() > max) {
                    e.consume();
                }
            }
        });

        B1 = new JButton("Wyślij");
        B2 = new JButton("Gotowy");

        B1.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String message =    L1.getText()+T1.getText().toLowerCase()+
                                    L2.getText()+T2.getText().toLowerCase()+
                                    L3.getText()+T3.getText().toLowerCase()+
                                    L4.getText()+T4.getText().toLowerCase()+
                                    L5.getText()+T5.getText().toLowerCase();
                BufferedReader bufred = new BufferedReader(new StringReader(message));
                try {
                    sock.getOutputStream().write(bufred.readLine().getBytes());
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
                B1.setEnabled(false);
                T1.setEditable(false);
                T2.setEditable(false);
                T3.setEditable(false);
                T4.setEditable(false);
                T5.setEditable(false);

            }
        } );
        B2.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                String message;
                if(B2.getText()=="Gotowy") {
                    message = "0";
                    B2.setText("Wycofaj");
                }else{
                    message = "1";
                    B2.setText("Gotowy");
                }
                BufferedReader bufred = new BufferedReader(new StringReader(message));
                try {
                    sock.getOutputStream().write(bufred.readLine().getBytes());
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        } );

        L1 = new JTextArea();
        L2 = new JTextArea();
        L3 = new JTextArea();
        L4 = new JTextArea();
        L5 = new JTextArea();

        JTextArea N1 = new JTextArea("Państwo");
        JTextArea N2 = new JTextArea("Miasto");
        JTextArea N3 = new JTextArea("Imie");
        JTextArea N4 = new JTextArea("Zwierze");
        JTextArea N5 = new JTextArea("Zawód");

        Tczas = new JTextArea();
        Twyniki = new JTextArea();

        L1.setEditable(false);
        L2.setEditable(false);
        L3.setEditable(false);
        L4.setEditable(false);
        L5.setEditable(false);

        N1.setEditable(false);
        N2.setEditable(false);
        N3.setEditable(false);
        N4.setEditable(false);
        N5.setEditable(false);

        L1.setFocusable(false);
        L2.setFocusable(false);
        L3.setFocusable(false);
        L4.setFocusable(false);
        L5.setFocusable(false);

        N1.setFocusable(false);
        N2.setFocusable(false);
        N3.setFocusable(false);
        N4.setFocusable(false);
        N5.setFocusable(false);

        TT.setFocusable(false);
        Tczas.setFocusable(false);
        Twyniki.setFocusable(false);

        Twyniki.setVisible(false);


        N1.setBackground(Color.lightGray);
        N2.setBackground(Color.lightGray);
        N3.setBackground(Color.lightGray);
        N4.setBackground(Color.lightGray);
        N5.setBackground(Color.lightGray);
        menuFrame.setBackground(Color.lightGray);

        Twyniki.setPreferredSize(new Dimension(ScWidth/8,ScWidth/16));
        Tczas.setColumns(7);
        T1.setColumns(10);
        T2.setColumns(10);
        T3.setColumns(10);
        T4.setColumns(10);
        T5.setColumns(10);
        L1.setColumns(1);
        L2.setColumns(1);
        L3.setColumns(1);
        L4.setColumns(1);
        L5.setColumns(1);

        T1.setLineWrap(true);
        T2.setLineWrap(true);
        T3.setLineWrap(true);
        T4.setLineWrap(true);
        T5.setLineWrap(true);
        Twyniki.setLineWrap(true);

        B1.setEnabled(false);
        T1.setEditable(false);
        T2.setEditable(false);
        T3.setEditable(false);
        T4.setEditable(false);
        T5.setEditable(false);

        layout.getConstraints(L1).setX(Spring.constant(ScWidth/20));
        layout.getConstraints(TT).setConstraint(SpringLayout.HORIZONTAL_CENTER,Spring.constant(ScWidth/2));
        layout.getConstraints(Tczas).setConstraint(SpringLayout.EAST,Spring.constant(ScWidth));


        layout.putConstraint(SpringLayout.NORTH, T1, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, T2, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, T3, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, T4, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, T5, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, L1, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, L2, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, L3, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, L4, ySpring,SpringLayout.SOUTH,TT);
        layout.putConstraint(SpringLayout.NORTH, L5, ySpring,SpringLayout.SOUTH,TT);

        layout.putConstraint(SpringLayout.WEST, T1,Spring.constant(-4),SpringLayout.EAST,L1);
        layout.putConstraint(SpringLayout.WEST, L2,Spring.constant(ScWidth/20),SpringLayout.EAST,T1);
        layout.putConstraint(SpringLayout.WEST, T2,Spring.constant(-4),SpringLayout.EAST,L2);
        layout.putConstraint(SpringLayout.WEST, L3,Spring.constant(ScWidth/20),SpringLayout.EAST,T2);
        layout.putConstraint(SpringLayout.WEST, T3,Spring.constant(-4),SpringLayout.EAST,L3);
        layout.putConstraint(SpringLayout.WEST, L4,Spring.constant(ScWidth/20),SpringLayout.EAST,T3);
        layout.putConstraint(SpringLayout.WEST, T4,Spring.constant(-4),SpringLayout.EAST,L4);
        layout.putConstraint(SpringLayout.WEST, L5,Spring.constant(ScWidth/20),SpringLayout.EAST,T4);
        layout.putConstraint(SpringLayout.WEST, T5,Spring.constant(-4),SpringLayout.EAST,L5);

        layout.putConstraint(SpringLayout.HORIZONTAL_CENTER, N1,Spring.constant(0),SpringLayout.HORIZONTAL_CENTER,T1);
        layout.putConstraint(SpringLayout.HORIZONTAL_CENTER, N2,Spring.constant(0),SpringLayout.HORIZONTAL_CENTER,T2);
        layout.putConstraint(SpringLayout.HORIZONTAL_CENTER, N3,Spring.constant(0),SpringLayout.HORIZONTAL_CENTER,T3);
        layout.putConstraint(SpringLayout.HORIZONTAL_CENTER, N4,Spring.constant(0),SpringLayout.HORIZONTAL_CENTER,T4);
        layout.putConstraint(SpringLayout.HORIZONTAL_CENTER, N5,Spring.constant(0),SpringLayout.HORIZONTAL_CENTER,T5);

        layout.putConstraint(SpringLayout.SOUTH, N1,Spring.constant(0),SpringLayout.NORTH,T1);
        layout.putConstraint(SpringLayout.SOUTH, N2,Spring.constant(0),SpringLayout.NORTH,T2);
        layout.putConstraint(SpringLayout.SOUTH, N3,Spring.constant(0),SpringLayout.NORTH,T3);
        layout.putConstraint(SpringLayout.SOUTH, N4,Spring.constant(0),SpringLayout.NORTH,T4);
        layout.putConstraint(SpringLayout.SOUTH, N5,Spring.constant(0),SpringLayout.NORTH,T5);

        layout.putConstraint(SpringLayout.NORTH, B1,Spring.constant(16),SpringLayout.SOUTH,T5);
        layout.putConstraint(SpringLayout.HORIZONTAL_CENTER, B1,Spring.constant(0),SpringLayout.HORIZONTAL_CENTER,T5);

        layout.putConstraint(SpringLayout.NORTH, B2,Spring.constant(16),SpringLayout.SOUTH,T5);
        layout.putConstraint(SpringLayout.HORIZONTAL_CENTER, B2,Spring.constant(0),SpringLayout.HORIZONTAL_CENTER,T1);

        layout.putConstraint(SpringLayout.WEST, Twyniki,Spring.constant(0),SpringLayout.WEST,L1);
        layout.putConstraint(SpringLayout.NORTH, Twyniki,Spring.constant(16),SpringLayout.SOUTH,B2);

        menuFrame.getContentPane().add(TT);
        menuFrame.getContentPane().add(Tczas);

        menuFrame.getContentPane().add(T1);
        menuFrame.getContentPane().add(T2);
        menuFrame.getContentPane().add(T3);
        menuFrame.getContentPane().add(T4);
        menuFrame.getContentPane().add(T5);

        menuFrame.getContentPane().add(L1);
        menuFrame.getContentPane().add(L2);
        menuFrame.getContentPane().add(L3);
        menuFrame.getContentPane().add(L4);
        menuFrame.getContentPane().add(L5);

        menuFrame.getContentPane().add(N1);
        menuFrame.getContentPane().add(N2);
        menuFrame.getContentPane().add(N3);
        menuFrame.getContentPane().add(N4);
        menuFrame.getContentPane().add(N5);

        menuFrame.getContentPane().add(B1);
        menuFrame.getContentPane().add(B2);

        menuFrame.getContentPane().add(Twyniki);

        menuFrame.setVisible(true);

    }


}

public class Main{


    public static void main(String[] args) {
        Socket sock = null;
        Socket sock2 = null;
        try {
            sock = new Socket("127.0.0.1",1234);
            sock2 = new Socket("127.0.0.1",1234);
        } catch (IOException e) {
            e.printStackTrace();
        }

        InputStream is = null;
        try {
            is = sock.getInputStream();
            is = sock2.getInputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }

        byte[] bytearr = new byte[256];

        GUI g = new GUI();
        g.Menu(sock);
        GUI h = new GUI();
        h.Menu(sock2);
        while(true){
            int len = 0;
            try {
                len = is.read(bytearr);
            } catch (IOException e) {
                e.printStackTrace();
            }
            if(len == -1) break;
            //System.out.write(bytearr, 0, len);
            String s = new String(bytearr, 0, len, Charset.forName("US-ASCII"));
            String s2="";
            //System.out.println(s);
            boolean erase = true;
            while(s.length()>0) {
                if (Character.isLowerCase(s.charAt(0)) == true) {
                    g.zmianaLitery(s.charAt(0));
                    h.zmianaLitery(s.charAt(0));
                    s = s.substring(1, s.length());
                }
                if(s.length() == 0) break;
                if(Character.isUpperCase(s.charAt(0)) == true){
                    int i;
                    for(i = 1;i<s.length()&&Character.isUpperCase(s.charAt(i))==false ;i++);
                    s2 = s.substring(0,i);
                    s = s.substring(i,s.length());
                }
                if (s2.substring(0, 1).equals("C")||s2.substring(0, 1).equals("S")) {
                    g.wyswietlCzas(s2);
                    h.wyswietlCzas(s2);
                } else if (s2.substring(0, 1).equals("G")){
                    if(erase == true){
                        g.wyczyscWyniki();
                        h.wyczyscWyniki();
                        erase = false;
                    }
                    g.wyswietlWyniki(s2);
                    h.wyswietlWyniki(s2);
                }else if(s2.substring(0, 1).equals("N")){
                    g.nowaGra();
                    h.nowaGra();
                }
            }
        }

        try {
            sock.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
