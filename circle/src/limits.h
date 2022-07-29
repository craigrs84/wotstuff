/* ************************************************************************
*  file: limits.h , Limit/Gain control module             Part of DIKUMUD *
*  Usage: declaration of title type                                       *
************************************************************************* */

/* Public Procedures */
int	mana_limit(struct char_data *ch);
int	hit_limit(struct char_data *ch);
int	move_limit(struct char_data *ch);
int	mana_gain(struct char_data *ch);
int	hit_gain(struct char_data *ch);
int	move_gain(struct char_data *ch);
void	advance_level(struct char_data *ch);
void	set_title(struct char_data *ch);
void	gain_exp(struct char_data *ch, int gain);
void	gain_exp_regardless(struct char_data *ch, int gain);
void	gain_condition(struct char_data *ch, int condition, int value);
void	check_idling(struct char_data *ch);
void	point_update( void );

struct title_type {
   char	*title_m;
   char	*title_f;
   int	exp;
};



