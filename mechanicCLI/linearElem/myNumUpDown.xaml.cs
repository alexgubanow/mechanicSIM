using System.Windows;
using System.Windows.Controls;

namespace linearElem
{
    /// <summary>
    /// Interaction logic for myNumUpDown.xaml
    /// </summary>
    public partial class myNumUpDown : UserControl
    {
        public myNumUpDown()
        {
            InitializeComponent();
        }

        public string ControlLabel
        {
            get { return (string)GetValue(_ControlLabel); }
            set { SetValue(_ControlLabel, value); }
        }

        public static readonly DependencyProperty _ControlLabel =
            DependencyProperty.Register("ControlLabel", typeof(string),
              typeof(myNumUpDown), new PropertyMetadata(""));

        public int ValueInt
        {
            get { return (int)GetValue(ValueIntProperty); }
            set { SetValue(ValueIntProperty, value); }
        }

        public static readonly DependencyProperty ValueIntProperty =
            DependencyProperty.Register("ValueInt", typeof(int),
              typeof(myNumUpDown), new PropertyMetadata(0));

        public int StepInc
        {
            get { return (int)GetValue(StepIncProperty); }
            set { SetValue(StepIncProperty, value); }
        }

        public static readonly DependencyProperty StepIncProperty =
            DependencyProperty.Register("StepInc", typeof(int),
              typeof(myNumUpDown), new PropertyMetadata(0));

        public int maximum
        {
            get { return (int)GetValue(_maximum); }
            set { SetValue(_maximum, value); }
        }

        public static readonly DependencyProperty _maximum =
            DependencyProperty.Register("maximum", typeof(int),
              typeof(myNumUpDown), new PropertyMetadata(0));

        public int minimum
        {
            get { return (int)GetValue(_minimum); }
            set { SetValue(_minimum, value); }
        }

        public static readonly DependencyProperty _minimum =
            DependencyProperty.Register("minimum", typeof(int),
              typeof(myNumUpDown), new PropertyMetadata(0));

        public Visibility labelVisability
        {
            get { return (Visibility)GetValue(_labelVisability); }
            set { SetValue(_labelVisability, value); }
        }

        public static readonly DependencyProperty _labelVisability =
            DependencyProperty.Register("labelVisability", typeof(Visibility),
              typeof(myNumUpDown), new PropertyMetadata(Visibility.Visible));

        private void PART_NumericUp_Click(object sender, RoutedEventArgs e)
        {
            if (ValueInt < maximum)
            {
                ValueInt += StepInc;
            }
        }

        private void PART_NumericDown_Click(object sender, RoutedEventArgs e)
        {
            if (ValueInt > minimum)
            {
                ValueInt -= StepInc;
            }
        }
    }
}