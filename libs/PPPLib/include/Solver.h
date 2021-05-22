//
// Created by cc on 7/23/20.
//

#ifndef PPPLIB_SOLVER_H
#define PPPLIB_SOLVER_H

#include "GnssFunc.h"
#include "GnssErrorModel.h"
#include "InsFunc.h"
#include "AdjFunc.h"
#include "GnssAR.h"
#include "OutSol.h"

using namespace Eigen;
namespace PPPLib{

    class cSolver {
    public:
        cSolver();
        virtual ~cSolver();

    private:
        int GetSingalInd(tSatObsUnit& sat_obs,int *f);
        int AdjustObsInd(tSatObsUnit& sat_obs,int* i,int* j,int* k);

    public:
        void InitEpochSatInfo(vector<tSatInfoUnit>& sat_infos);
        void UpdateSatInfo(vector<tSatInfoUnit>& sat_infos);
        void UpdateGnssObs(tPPPLibConf C,tEpochSatUnit& epoch_sat,RECEIVER_INDEX rec);
        void CorrGnssObs(tPPPLibConf C,Vector3d& rr);
        virtual int GnssObsRes(int post,tPPPLibConf C,double* x);

        virtual void InitSolver(tPPPLibConf C);
        virtual bool InitReader(tPPPLibConf C);
        virtual bool SolverProcess(tPPPLibConf C,int idx);
        virtual bool SolverEpoch();
        virtual bool Estimator(tPPPLibConf C);
        virtual bool SolutionUpdate();

        void CloseSolver();

        void InitInsPx(tPPPLibConf C,int nx,MatrixXd& Px);
        void InitX(double xi,double var,int idx,double *x,double *xp);
        Eigen::MatrixXd InitQ(tPPPLibConf,double dt,int nx);

        void RemoveLever(const tImuInfoUnit& imu_info,Vector3d& lever,Vector3d& gnss_re,Vector3d& gnss_ve);
        void CloseLoopState(VectorXd& x,tImuInfoUnit* imu_info_corr);


    public:
        cGnssObsOperator gnss_obs_operator_;
        cGnssErrCorr gnss_err_corr_;
        cParSetting para_;
        cLsqAdjuster lsq_;
        cKfAdjuster kf_;
        cOutSol *out_;

    public:
        int epoch_idx_=0;
        int epoch_ok_=0;
        int epoch_fail_=0;
        tNav nav_;
        cGnssObs rover_obs_;
        cGnssObs base_obs_;
        vector<tSolInfoUnit> ref_sols_;

        tEpochSatUnit epoch_sat_obs_;
        tEpochSatUnit base_epoch_sat_obs_;

        vector<tSatInfoUnit> epoch_sat_info_collect_;
        vector<tSatInfoUnit> base_sat_info_collect_;

        vector<int>vflag_;
        tSolInfoUnit ppplib_sol_;
        tSatInfoUnit previous_sat_info_[MAX_SAT_NUM];
        cLambda lambda_;

    public:
        int num_full_x_;
        VectorXd full_x_;
        MatrixXd full_Px_;
        int num_valid_sat_;
        int num_L_;
        VectorXd omc_L_;
        MatrixXd H_;
        MatrixXd R_;
        MatrixXd F_; //for coupled
        int sys_mask_[NSYS+1]={0};
        int num_real_x_fix_;
        VectorXd real_x_fix_;
        MatrixXd real_Px_fix_;
        vector<double>code_sigma,phase_sigma;

        bool tc_mode_=false;
        tImuInfoUnit cur_imu_info_;
    };

    class cSppSolver:public cSolver {
    public:
        cSppSolver();
        cSppSolver(tPPPLibConf conf);
        ~cSppSolver();

    private:
        void CorrDoppler(tSatInfoUnit& sat_info, Vector3d& rover_xyz,int f);
        Vector3d SatVelSagnacCorr(const Vector3d& sat_pos,const double tau);
        int DopplerRes(tPPPLibConf C,MatrixXd& H_mat, MatrixXd& R_mat,VectorXd& L,VectorXd& x,Vector3d rover_xyz);
        void EstDopVel(Vector3d rover_xyz);

        double Dops();
        bool ValidateSol(tPPPLibConf C);
        bool PostResidualQc(vector<double>omcs,vector<double>R);
        bool RaimFde();

    public:
        int GnssObsRes(int post,tPPPLibConf C,double* x) override;
        void InitSolver(tPPPLibConf C) override;
        bool SolverProcess(tPPPLibConf C,int idx) override;
        bool SolverEpoch() override;
        bool Estimator(tPPPLibConf C) override;
        bool SolutionUpdate() override;

    private:
        int iter_=10;
    public:
        tPPPLibConf spp_conf_;
    };

    class cTdcpSolver:public cSolver {
    public:
        cTdcpSolver();
        cTdcpSolver(tPPPLibConf conf);
        ~cTdcpSolver();

    public:
        tPPPLibConf  tdcp_conf_;
    };

    class cPppSolver:public cSolver {
    public:
        cPppSolver();
        cPppSolver(tPPPLibConf C);
        ~cPppSolver();

    public:
        void InitSolver(tPPPLibConf C) override;
        bool SolverProcess(tPPPLibConf C,int idx) override;
        bool SolverEpoch() override;
        bool Estimator(tPPPLibConf C) override;
        bool SolutionUpdate() override;

    private:
        void InitSppSolver();
        void Spp2Ppp();
        void PppCycSlip(tPPPLibConf C);
        void ClockJumpRepair();
        void PosUpdate(tPPPLibConf C);
        void ClkUpdate(tPPPLibConf C,double tt);
        void PhaseClkUpdate(tPPPLibConf C,double tt);
        void IfbUpdate(tPPPLibConf C,double tt);
        void GloIfcbUpdate(tPPPLibConf C,double tt);
        void TrpUpdate(tPPPLibConf C,double tt);
        void IonUpdate(tPPPLibConf C,double tt);
        void AmbUpdate(tPPPLibConf C,double tt);
        void StateTimeUpdate(tPPPLibConf C);
        int GnssObsRes(int post,tPPPLibConf C,double *x,Vector3d re);
        void ReInitPppSolver(tPPPLibConf C);

        void DisableX(int iter,VectorXd& x,vector<int>&par_idx,vector<double>& back_values);

        // quality control
        bool PppResidualQc(int iter,vector<double>omcs,vector<double>var);

        // PPP-AR
        bool ResolvePppAmbRotRef(int nf,VectorXd& x,MatrixXd& P);
        // PPP-AR select high-ele satellite
        bool ResolvePppAmbFixRef(int nf,VectorXd& x,MatrixXd& P);

        bool FixPppSol(int *sat1,int *sat2,double *NC,int n,VectorXd& x,MatrixXd& P);
        bool AmbLinearDependCheck(int sat1,int sat2,int *flag,int *max_flg);
        int SelectAmb(int *sat1,int *sat2,double *N,double *var,int n);

        // fix wide-lane ambiguity
        void AverageLcAmb();
        bool FixWlAmb(int sat1,int sat2,int *fix_wl,double *res_wl);

        // fix narrow-lane ambiguity with CNES IRC
        bool FixNlAmbILS_IRC(int *sat1,int *sat2,int *fix_wls,double *res_wls,int n,VectorXd& x,MatrixXd& P);

        // fix narrow-lane ambiguity with SGG FCB
        bool MatchNlFcb(int sat1,int sat2,double *nl_fcb1,double *nl_fcb2);
        bool FixNlAmbILS_FCB(int *sat1,int *sat2,int *fix_wls,double *res_wls,int n,VectorXd& x,MatrixXd& P);

        // ppp-ar use ppk-ar function
        void ResetAmb(double *bias,double *xa,int nb);
        bool FixNlAmbILS1(int *sat1,int *sat2,int *fix_wls,int n,double *xa);
        int SelectFixSat(double *D,int gps,int glo);
        int ResolveAmbLambda(double *xa,int gps,int glo);
        bool ResolverPppAmb1(int nf,double *xa);
    private:
        tPPPLibConf ppp_conf_;
        int max_iter_=8;
        vector<tSdAmb> sdambs_;
        cTime filter_start_;
        bool reset_flag_=false;

    public:
        cSppSolver *spp_solver_;
        int exc_sat_index_=0;
        double pre_epoch_ar_ratio1_=0.0;
        double pre_epoch_ar_ratio2_=0.0;

    };

    class cPpkSolver:public cSolver {
    public:
        cPpkSolver();
        cPpkSolver(tPPPLibConf C);
        ~cPpkSolver();

    public:
        void InitSolver(tPPPLibConf C) override;
        bool SolverProcess(tPPPLibConf C,int idx) override;
        bool SolverEpoch() override;
        bool Estimator(tPPPLibConf C) override;
        bool SolutionUpdate() override;

    private:
        void InitSppSolver();
        void Spp2Ppk();
        bool GnssZeroRes(tPPPLibConf C,RECEIVER_INDEX rec,vector<int>sat_idx,double* x,Vector3d rr);
        int GnssDdRes(int post,tPPPLibConf C,vector<int>ir,vector<int>ib,vector<int>cmn_sat_no,double* x,int refsat[NSYS][2*MAX_GNSS_USED_FRQ_NUM]);
        bool ValidObs(int i,int nf,int f);
        bool MatchBaseObs(cTime t);
        int SelectCmnSat(tPPPLibConf C,vector<int>& ir,vector<int>& iu,vector<int>& cmn_sat_no);
        void PpkCycleSlip(tPPPLibConf C,vector<int>& iu,vector<int>& ib,vector<int>& cmn_sat_no);
        void StateTimeUpdate(tPPPLibConf C,vector<int>& iu,vector<int>& ib,vector<int>& cmn_sat_no);
        void PosUpdate(tPPPLibConf C);
        void GloIfpbUpdate(tPPPLibConf C,double tt);
        void TrpUpdate(tPPPLibConf C,double tt);
        void IonUpdate(tPPPLibConf C,double tt);
        void AmbUpdate(tPPPLibConf C, double tt,vector<int>& ir,vector<int>& ib,vector<int>& cmn_sat_no);

        bool CheckDualFrq(tSatInfoUnit& sat_info);
        bool PpkResidualQc(int iter,vector<int>ir,vector<int> cmn_sat,vector<double>& omcs, vector<double>R);

        void ReSetAmb(double *bias,double *xa,int nb);
        int DdMat(double *D,int gps,int bds,int glo,int gal);
        int ResolveAmbLambda(double *xa,int gps, int bds,int glo,int gal, int qzs);
        bool ResolvePpkAmb(vector<int>cmn_sat,int nf,double *xa);
        void HoldAmb(vector<int>cmn_sat,double *xa);

    private:
        cSppSolver *spp_solver_;
        tPPPLibConf ppk_conf_;
        Vector3d base_xyz_;

    private:
        double tt_=0.0;
        int base_idx_=0;
        vector<double>base_res,rover_res;
        vector<tDdAmb> ddambs_;
        int exc_sat_index=0;
        int num_continuous_fix_=0;
        bool amb_hold_flag=false;
        double pre_epoch_ar_ratio1=0.0;
        double pre_epoch_ar_ratio2=0.0;
        int qc_iter_=0;
    };

    class cFusionSolver:public cSolver {
    public:
        cFusionSolver();
        cFusionSolver(tPPPLibConf C);
        ~cFusionSolver();

    private:
        bool InputImuData(int ws);
        bool MatchGnssObs();
        bool MatchGnssSol();
        void InsSol2PpplibSol(tImuInfoUnit &imu_sol,tSolInfoUnit &ppplib_sol);

        double Vel2Yaw(Vector3d vn);
        bool GnssSol2Ins(Vector3d re,Vector3d ve);
        Vector3d Pos2Vel(tSolInfoUnit& sol1,tSolInfoUnit& sol2);
        bool InsAlign();

    public:
        void InitSolver(tPPPLibConf C) override;
        bool SolverProcess(tPPPLibConf C,int idx) override;
        bool SolverEpoch() override;
        bool SolutionUpdate() override;

    private:
        void DisableX(VectorXd& x,int idx);
        void StateTimeUpdate();
        void PropVariance(MatrixXd& F,MatrixXd& Q,int nx,MatrixXd& Px);
        int BuildLcHVR(int post,tPPPLibConf C,tImuInfoUnit& imu_info,double *meas_pos,double *meas_vel,Vector3d& q_pos,Vector3d& q_vel);
        bool LcFilter(tPPPLibConf C);
        bool ValidSol(VectorXd& x, double thres);
        void ControlPx(int nx,MatrixXd& Px);

    public:
        bool LooseCouple(tPPPLibConf C);
        bool TightCouple(tPPPLibConf C);

    private:
        cInsMech ins_mech_;
        cSolver *gnss_alignor_;
        cSolver *gnss_solver_;
        tPPPLibConf fs_conf_;
        tPPPLibConf gnss_conf_;

    private:
        cImuData imu_data_;
        vector<tSolInfoUnit> gnss_sols_;

    private:
        int imu_index_=0;
        int rover_idx_=0;
        int base_idx_=0;
        int gnss_sol_idx=0;
        int ins_mech_idx=0;
        tImuInfoUnit cur_imu_info_={0};
        tImuInfoUnit pre_imu_info_={0};
        vector<tImuDataUnit> imu_data_zd_;
    };
}




#endif //PPPLIB_SOLVER_H
